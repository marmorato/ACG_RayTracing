
#include "neepathtracing.h"
#include "../core/utils.h"
#include "../materials/material.h"
#include "../lightsources/arealightsource.h"

#include <algorithm>
#include <cmath>

// CONSTRUCTOR
NEEPathTracing::NEEPathTracing()
    : Shader(Vector3D(0.0))
{
}


// 1. COMPUTE RADIANCE: Le + Lr
Vector3D NEEPathTracing::computeColor(
    const Ray &r,
    const std::vector<Shape*> &objList,
    const std::vector<LightSource*> &lsList
) const
{
    Intersection its;

    if (!Utils::getClosestIntersection(r, objList, its)) {
        return bgColor;
    }

    Vector3D wo = -r.d;
    const Material& material = its.shape->getMaterial();

    // Emitted radiance
    if (material.isEmissive()) {
        return material.getEmissiveRadiance();
    }

    // Maximum depth
    if (r.depth >= 5) {
        return Vector3D(0.0);
    }

    // Reflected radiance
    return reflectedRadiance(its, wo, r.depth, objList, lsList);
}


// 2. REFLECTED RADIANCE: Ldir + Lind
Vector3D NEEPathTracing::reflectedRadiance(
    const Intersection &its,
    const Vector3D &wo,
    int depth,
    const std::vector<Shape*> &objList,
    const std::vector<LightSource*> &lsList
) const
{
    Vector3D Ldir = directRadiance(its, wo, objList, lsList);

    Vector3D Lind = indirectRadiance(its, wo, depth, objList, lsList);

    return Ldir + Lind;
}


// 3. DIRECT RADIANCE: Area Sampling
Vector3D NEEPathTracing::directRadiance(
    const Intersection &its,
    const Vector3D &wo,
    const std::vector<Shape*> &objList,
    const std::vector<LightSource*> &lsList
) const
{
    Vector3D p = its.itsPoint;
    Vector3D n = its.normal;

    const Material& material = its.shape->getMaterial();

    Vector3D directColor(0.0);

    for (LightSource* light : lsList) {

        AreaLightSource* areaLight = static_cast<AreaLightSource*>(light);

        // Random position on the light
        Vector3D y = areaLight->sampleLightPosition();

        Vector3D lightDir = y - p;
        double distance = lightDir.length();

        if (distance <= Epsilon) {
            continue;
        }

        Vector3D wi = lightDir / distance;

        // Visibility check
        Ray shadowRay(p, wi, 0, Epsilon, distance - Epsilon);

        if (Utils::hasIntersection(shadowRay, objList)) {
            continue;
        }

        // Geometric factor
        Vector3D ny = areaLight->getNormal();

        double cosThetaSurface = std::max(0.0, dot(n, wi));
        double cosThetaLight = std::max(0.0, dot(ny, -wi));

        double G = (cosThetaSurface * cosThetaLight)
                   / (distance * distance);

        // BRDF and emitted radiance
        Vector3D Li = areaLight->getIntensity();
        Vector3D fr = material.getReflectance(n, wo, wi);

        double probabilityd = 1.0 / areaLight->getArea();

        directColor += Li * fr * (G / probabilityd);
    }

    return directColor;
}


// 4. INDIRECT RADIANCE: To implement next
Vector3D NEEPathTracing::indirectRadiance(
    const Intersection &its,
    const Vector3D &wo,
    int depth,
    const std::vector<Shape*> &objList,
    const std::vector<LightSource*> &lsList
) const
{
    // Stop the recursion
    if (depth >= 5) {
        return Vector3D(0.0);
    }

    Vector3D p = its.itsPoint;
    Vector3D n = its.normal;

    const Material& material = its.shape->getMaterial();

    // Sample a random direction
    Vector3D wi = sampler.getSample(n);

    Ray nextRay(p, wi, depth + 1, Epsilon, 1e30);

    // Find the next intersection
    Intersection nextIts;

    if (!Utils::getClosestIntersection(nextRay, objList, nextIts)) {
        return Vector3D(0.0);
    }

    // Only calculate reflected light at the next surface
    // This avoids counting direct light twice
    if (nextIts.shape->getMaterial().isEmissive()) {
        return Vector3D(0.0);
    }

    Vector3D Li = reflectedRadiance(
        nextIts, -nextRay.d, depth + 1, objList, lsList
    );

    Vector3D fr = material.getReflectance(n, wo, wi);

    double cosTheta = std::max(0.0, dot(n, wi));

    double probabilityd = 1.0 / (2.0 * M_PI);

    return Li * fr * (cosTheta / probabilityd);
}

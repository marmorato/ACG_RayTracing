//
//  hemisfericaldirectintegrator.cpp
//  ACG
//
//  Created by mar morató aguilar on 23/10/26.
//

#include "hemisphericaldirectintegrator.h"
#include "../core/utils.h"
#include "../materials/material.h"

HemisphericalDirectIntegrator::HemisphericalDirectIntegrator()
    : Shader(Vector3D(0.0))
{
}

Vector3D HemisphericalDirectIntegrator::computeColor(
    const Ray &r,
    const std::vector<Shape*> &objList,
    const std::vector<LightSource*> &lsList
) const
{
    Intersection its;

    if (!Utils::getClosestIntersection(r, objList, its)) {
        return bgColor;
    }

    Vector3D p = its.itsPoint;
    Vector3D n = its.normal;
    Vector3D wo = -r.d;

    const Material& material = its.shape->getMaterial();
    
    // EMISSIVE MATERIAL
    // If the ray directly hits a light, return its emitted radiance
    if (material.isEmissive()) {
        return material.getEmissiveRadiance();
    }

    // MIRROR MATERIAL (Lab 1)
    if (material.hasSpecular()) {

        if (r.depth >= 5) {
            return bgColor;
        }

        Vector3D wr = 2.0 * dot(n, wo) * n - wo;

        Ray reflectionRay(p, wr, r.depth + 1, Epsilon, 1e30);

        return computeColor(reflectionRay, objList, lsList);
    }

    // TRANSMISSIVE MATERIAL (GLASS Lab 1))
    if (material.hasTransmission()) {

        if (r.depth >= 5) {
            return bgColor;
        }

        double mu = material.getIndexOfRefraction();
        Vector3D nx = n;
        double dotProd = dot(nx, wo);

        // Check if the ray enters or leaves the object
        if (dotProd < 0.0) {
            nx = -n;
            dotProd = dot(nx, wo);
            mu = 1.0 / mu;
        }

        double insqrt = 1.0 - mu * mu * (1.0 - dotProd * dotProd);

        Vector3D newDirection;

        if (insqrt < 0.0) {
            // Total internal reflection
            newDirection = 2.0 * dotProd * nx - wo;
        } else {
            // Refraction
            newDirection = -mu * wo
                         + nx * (mu * dotProd - std::sqrt(insqrt));
        }

        Ray secondaryRay(p, newDirection, r.depth + 1, Epsilon, 1e30);

        return computeColor(secondaryRay, objList, lsList);
    }

    // HEMISPHERICAL DIRECT ILLUMINATION LAB 2
    // Generate a random direction over the hemisphere
    Vector3D wi = sampler.getSample(n);

    Ray sampleRay(p, wi, 0, Epsilon, 1e30);
    Intersection sampleIts;


    if (Utils::getClosestIntersection(sampleRay, objList, sampleIts)) {


        if (sampleIts.shape->getMaterial().isEmissive()) {

            Vector3D Li = sampleIts.shape->getMaterial().getEmissiveRadiance();

            // BRDF of the original surface
            Vector3D fr = material.getReflectance(n, wo, wi);

            double cosTheta = std::max(0.0, dot(n, wi));

            double probabilityd = 1.0 / (2.0 * M_PI);

            Vector3D contribution = Li * fr * (cosTheta / probabilityd);

            return contribution;
        }
    }

    // The sampled direction did not reach a light
    return Vector3D(0.0);
}


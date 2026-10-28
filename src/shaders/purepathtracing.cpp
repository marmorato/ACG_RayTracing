//
//  purepathtracing.cpp
//  ACG
//
//  Created by mar morató aguilar on 28/10/26.
//

#include "purepathtracing.h"
#include "../core/utils.h"
#include "../materials/material.h"

#include <algorithm>
#include <cmath>

PurePathTracing::PurePathTracing()
    : Shader(Vector3D(0.0))
{
}

Vector3D PurePathTracing::computeColor(
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

    // If the ray hits a light, return its emitted radiance
    if (material.isEmissive()) {
        return material.getEmissiveRadiance();
    }
    if (r.depth >= 5) {
            return Vector3D(0.0);
        }

    Vector3D wi = sampler.getSample(n);
    // MIRROR MATERIAL
    if (material.hasSpecular()) {

        Vector3D wr = 2.0 * dot(n, wo) * n - wo;

        Ray reflectionRay(p, wr, r.depth + 1, Epsilon, 1e30);

        return computeColor(reflectionRay, objList, lsList);
    }

       // TRANSMISSIVE MATERIAL (GLASS)
    if (material.hasTransmission()) {

        double mu = material.getIndexOfRefraction();
        Vector3D nx = n;
        double dotProd = dot(nx, wo);

        // Check whether the ray is entering or leaving
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
           }
        else {
            // Refraction
            newDirection = -mu * wo + nx * (mu * dotProd - std::sqrt(insqrt));
           }

        Ray secondaryRay(p, newDirection, r.depth + 1, Epsilon, 1e30);

        return computeColor(secondaryRay, objList, lsList);
       }

    // Create a new ray in that direction
    Ray nextRay(p, wi, r.depth + 1, Epsilon, 1e30);

    // Recursively calculate the incoming radiance
    Vector3D Li = computeColor(nextRay, objList, lsList);

    Vector3D fr = material.getReflectance(n, wo, wi);

    double cosTheta = std::max(0.0, dot(n, wi));

    double probabilityd = 1.0 / (2.0 * M_PI);

    Vector3D contribution = Li * fr * (cosTheta / probabilityd);

    return contribution;
}

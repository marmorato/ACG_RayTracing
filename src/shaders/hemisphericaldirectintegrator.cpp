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
    
    // If the camera ray hits a light return its emitted radiance
    if (material.isEmissive()) {
        return material.getEmissiveRadiance();
    }
    
    // generate random direction over the hemisphere
    Vector3D wi = sampler.getSample(n);
    Ray sampleRay(p, wi, 0, Epsilon, 1e30);
    Intersection sampleIts;

    if (Utils::getClosestIntersection(sampleRay, objList, sampleIts)) {
        if (sampleIts.shape->getMaterial().isEmissive()) {
            Vector3D Li = sampleIts.shape->getMaterial().getEmissiveRadiance();
            Vector3D fr = material.getReflectance(n, wo, wi);
            double cosTheta = std::max(0.0, dot(n, wi));
            double probabilityd = 1.0 / (2.0 * M_PI);
            
            Vector3D contribution = Li * fr * (cosTheta / probabilityd);
            
            return contribution;
        }
    }

    return Vector3D(0.0); // No direct light reached
}

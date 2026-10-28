//
//  areadirectintegrator.cpp
//  ACG
//
//  Created by mar morató aguilar on 27/10/26.
//

#include "areadirectintegrator.h"
#include "../core/utils.h"
#include "../materials/material.h"


#include <algorithm>
#include <cmath>

AreaDirectIntegrator::AreaDirectIntegrator()
    : Shader(Vector3D(0.0))
{
}

Vector3D AreaDirectIntegrator::computeColor(
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

    if (material.isEmissive()) {
        return material.getEmissiveRadiance();
    }
    
    // MIRROR MATERIAL
    if (material.hasSpecular()) {

        if (r.depth >= 5) {
            return bgColor;
        }

        Vector3D wr = 2.0 * dot(n, wo) * n - wo;

        Ray reflectionRay(p, wr, r.depth + 1, Epsilon, 1e30);

        return computeColor(reflectionRay, objList, lsList);
        }

        // TRANSMISSIVE MATERIAL (GLASS)
    if (material.hasTransmission()) {

        if (r.depth >= 5) {
            return bgColor;
        }

        double mu = material.getIndexOfRefraction();
        Vector3D nx = n;
        double dotProd = dot(nx, wo);

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
            newDirection = -mu * wo+ nx * (mu * dotProd - std::sqrt(insqrt));
        }

        Ray secondaryRay(p, newDirection, r.depth + 1, Epsilon, 1e30);

        return computeColor(secondaryRay, objList, lsList);
        }

    Vector3D color(0.0);

    for (LightSource* light : lsList) {

        Vector3D y = light->sampleLightPosition();

        Vector3D lightDir = y - p;
        double distance = lightDir.length();

        if (distance <= Epsilon) {
            continue;
        }

        Vector3D wi = lightDir / distance;
        
        Ray shadowRay(p, wi, 0, Epsilon, distance - Epsilon);

        if (Utils::hasIntersection(shadowRay, objList)) {
            continue;
        }
        
        //G(x,y)
        AreaLightSource* areaLight = static_cast<AreaLightSource*>(light);
        Vector3D ny = areaLight->getNormal();

        double cosThetaSurface = std::max(0.0, dot(n, wi));
        double cosThetaLight = std::max(0.0, dot(ny, -wi));
        double G = (cosThetaSurface * cosThetaLight) / (distance * distance);
        
        Vector3D Li = light->getIntensity();

        Vector3D fr = material.getReflectance(n, wo, wi);

        double probabilityd = 1.0 / areaLight->getArea();

        Vector3D contribution = Li * fr * (G / probabilityd);

        color += contribution;
    }
    
    

    return color;
}

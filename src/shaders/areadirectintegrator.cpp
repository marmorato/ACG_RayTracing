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

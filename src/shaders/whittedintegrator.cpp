//
//  whittedintegrator.cpp
//  ACG
//
//  Created by mar morató aguilar on 13/10/26.
//


#include "whittedintegrator.h"
#include "../core/utils.h"
#include "../materials/material.h"

#include <algorithm>

WhittedIntegrator::WhittedIntegrator()
    : Shader(Vector3D(0.0, 0.0, 0.0))
{
}

Vector3D WhittedIntegrator::computeColor(
    const Ray &r,
    const std::vector<Shape*> &objList,
    const std::vector<LightSource*> &lsList
) const
{
    Intersection its;

    if (!Utils::getClosestIntersection(r, objList, its)) {
        return bgColor; //check if the ray intersects an object
    }

    Vector3D p = its.itsPoint;
    Vector3D n = its.normal;
    Vector3D wo = -r.d;

    const Material& material = its.shape->getMaterial(); //to know the material of the intersection

    Vector3D color(0.0); //initializing the color

    // Ambient illumination
    double ambient = 0.1;
    color = material.getDiffuseReflectance() * ambient;

    // looping trhough all the light sources in the sceneyes
    for (LightSource* light : lsList) {

        Vector3D lightPos = light->sampleLightPosition();
        Vector3D lightDir = lightPos - p;

        double distance = lightDir.length();
        Vector3D wi = lightDir / distance; //from x to current light

        // Shadow ray
        Ray shadowRay(p, wi, 0, Epsilon, distance - Epsilon); // we used epsilon so that when we check if sometihn intersects btween the obkect and the light, we dont take into account the own point or light

        double visibility = 1.0;

               if (Utils::hasIntersection(shadowRay, objList)) {
                   visibility = 0.0;
               }

                //wi*nx
               double dotProduct = n.x * wi.x + n.y * wi.y + n.z * wi.z;

               // calculate the material reflectance
               Vector3D reflectance = material.getReflectance(n, wo, wi);

               // get the light intensity
               Vector3D lightIntensity = light->getIntensity();

               // add the ligth contribution
        color += lightIntensity * reflectance * dotProduct * visibility;
    }


    return color;
}

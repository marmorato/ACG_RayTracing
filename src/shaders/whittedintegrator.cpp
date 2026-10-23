#include "whittedintegrator.h"
#include "../core/utils.h"
#include "../materials/material.h"

#include <algorithm>

WhittedIntegrator::WhittedIntegrator()
    : Shader(Vector3D(0.0, 0.0, 0.0))
{
}

Vector3D WhittedIntegrator::computeColor(const Ray &r,const std::vector<Shape*> &objList, const std::vector<LightSource*> &lsList) const {
    
    Intersection its;

    if (!Utils::getClosestIntersection(r, objList, its)) {
        return bgColor; //check if the ray intersects an object
    }

    Vector3D p = its.itsPoint;
    Vector3D n = its.normal;
    Vector3D wo = -r.d;

    const Material& material = its.shape->getMaterial(); //to know the material of the intersection

    //MIRROR MATERIAL
    if (material.hasSpecular()) {

        //limit reflection depth
        if (r.depth >= 5) {
            return bgColor;
        }

        //compute reflection direction
        Vector3D wr = 2.0 * dot(n, wo) * n - wo;

        //create reflected ray and return compute color()
        Ray reflectionRay(p, wr, r.depth + 1, Epsilon, 1e30);
        return computeColor(reflectionRay, objList, lsList);
    }

    Vector3D color(0.0); //initializing the color
    
    //TRANSMISSIVE MATERIAL
    if (material.hasTransmission()) {

        //limit recursion depth
        if (r.depth >= 5) {
            return bgColor;
        }

        double mu = material.getIndexOfRefraction();
        //get the surface normal and viewing direction
        Vector3D nx = n;
        Vector3D wo = -r.d;
        //dot product --> normal and viewing direction
        double dotProd = dot(nx,wo);
        
        //check if the ray is entering or leaving the object
        if (dotProd < 0.0) {
            nx = -n;
            dotProd = dot(nx,wo);
            mu = 1.0 / mu;
        }

        double insqrt = 1.0 - mu * mu * (1.0 - dotProd * dotProd);

        Vector3D newDirection;
        
        //check for total internal reflection
        if (insqrt < 0.0) {
            //reflected direction
            newDirection = 2.0 * dotProd * nx - wo;

        } else {
            //refracted direction
            newDirection = -mu * wo + nx * (mu * dotProd - std::sqrt(insqrt));
        }
        
        //secondary ray with the new direction
        Ray secondaryRay(p,newDirection,r.depth + 1,Epsilon,1e30);

        return computeColor(secondaryRay, objList, lsList);
    }



    // Ambient illumination
    double ambient = 0.15;
    color = material.getDiffuseReflectance() * ambient;

    // looping trhough all the light sources in the scenes
    if (material.hasDiffuseOrGlossy()) {
        for (LightSource* light : lsList) {

            Vector3D lightPos = light->sampleLightPosition();
            Vector3D lightDir = lightPos - p;

            double distance = lightDir.length();

            if (distance <= Epsilon) {
                continue;
            }

            Vector3D wi = lightDir / distance; //from x to current light

            // Shadow ray
            Ray shadowRay(p, wi, 0, Epsilon, distance - Epsilon); // we used epsilon so that when we check if sometihn intersects btween the obkect and the light, we dont take into account the own point or light

            double visibility = 1.0;

            if (Utils::hasIntersection(shadowRay, objList)) {
                visibility = 0.0;
            }

            //wi*nx
            double dotProduct = dot(n,wi);

            if (dotProduct <= 0.0) {
                continue;
            }

            if (visibility <= 0.0) {
                continue;
            }

            // calculate the material reflectance
            Vector3D reflectance = material.getReflectance(n, wo, wi);

            // get the light intensity
            Vector3D lightIntensity = light->getIntensity();

            // add the ligth contribution
            color += lightIntensity * reflectance * dotProduct * visibility;
        }
    }

    return color;
}

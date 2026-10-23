//
//  hemisfericaldirectintegrator.cpp
//  ACG
//
//  Created by mar morató aguilar on 23/10/26.
//

#include "hemisphericaldirectintegrator.h"
#include "../core/utils.h"

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
    return Vector3D(0.0);
}

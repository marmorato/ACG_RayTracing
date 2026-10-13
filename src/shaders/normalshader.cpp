//
//  normalshader.cpp
//  ACG
//
//  Created by mar morató aguilar on 13/10/26.
//

#include "normalshader.h"
#include "../core/utils.h"

NormalShader::NormalShader() : Shader(Vector3D(0.0, 0.0, 0.0))
{
}

Vector3D NormalShader::computeColor(const Ray &r,const std::vector<Shape*> &objList,const std::vector<LightSource*> &lsList) const
{
    Intersection its;

    if (Utils::getClosestIntersection(r, objList, its)) {
        Vector3D normal = its.normal;
        return (normal + Vector3D(1.0, 1.0, 1.0)) / 2.0;
    }

    return bgColor;
}

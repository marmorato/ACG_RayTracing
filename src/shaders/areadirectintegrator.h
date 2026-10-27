//
//  areadirectintegrator.h
//  ACG
//
//  Created by mar morató aguilar on 27/10/26.
//

#ifndef AREADIRECTINTEGRATOR_H
#define AREADIRECTINTEGRATOR_H

#include "shader.h"

class AreaDirectIntegrator : public Shader
{
public:
    AreaDirectIntegrator();

    Vector3D computeColor(
        const Ray &r,
        const std::vector<Shape*> &objList,
        const std::vector<LightSource*> &lsList
    ) const;
};

#endif

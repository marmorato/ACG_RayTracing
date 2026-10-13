//
//  whittedintegrator.h
//  ACG
//
//  Created by mar morató aguilar on 13/10/26.
//

#ifndef WHITTEDINTEGRATOR_H
#define WHITTEDINTEGRATOR_H

#include "shader.h"

class WhittedIntegrator : public Shader
{
public:
    WhittedIntegrator();

    Vector3D computeColor(const Ray &r,const std::vector<Shape*> &objList,const std::vector<LightSource*> &lsList
    ) const;
};

#endif

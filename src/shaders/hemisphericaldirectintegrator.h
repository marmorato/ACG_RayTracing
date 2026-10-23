//
//  hemisfericaldirectintegrator.h
//  ACG
//
//  Created by mar morató aguilar on 23/10/26.
//

#ifndef HEMISPHERICALDIRECTINTEGRATOR_H
#define HEMISPHERICALDIRECTINTEGRATOR_H

#include "shader.h"
#include "../core/hemisphericalsampler.h"

class HemisphericalDirectIntegrator : public Shader
{
public:
    HemisphericalDirectIntegrator();

    Vector3D computeColor(
        const Ray &r,
        const std::vector<Shape*> &objList,
        const std::vector<LightSource*> &lsList
    ) const;

private:
    HemisphericalSampler sampler;
};

#endif

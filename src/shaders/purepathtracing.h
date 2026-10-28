//
//  purepathtracing.h
//  ACG
//
//  Created by mar morató aguilar on 28/10/26.
//

#ifndef PUREPATHTRACING_H
#define PUREPATHTRACING_H

#include "shader.h"
#include "../core/hemisphericalsampler.h"

class PurePathTracing : public Shader
{
public:
    PurePathTracing();

    Vector3D computeColor(
        const Ray &r,
        const std::vector<Shape*> &objList,
        const std::vector<LightSource*> &lsList
    ) const;

private:
    HemisphericalSampler sampler;
};

#endif

//
//  neepathtracing.h
//  ACG
//
//  Created by mar morató aguilar on 28/10/26.
//


#ifndef NEEPATHTRACING_H
#define NEEPATHTRACING_H

#include "shader.h"
#include "../core/hemisphericalsampler.h"
#include "../core/intersection.h"

class NEEPathTracing : public Shader
{
public:
    NEEPathTracing();

    Vector3D computeColor(
        const Ray &r,
        const std::vector<Shape*> &objList,
        const std::vector<LightSource*> &lsList
    ) const;

private:
    HemisphericalSampler sampler;

    Vector3D reflectedRadiance(
        const Intersection &its,
        const Vector3D &wo,
        int depth,
        const std::vector<Shape*> &objList,
        const std::vector<LightSource*> &lsList
    ) const;

    Vector3D directRadiance(
        const Intersection &its,
        const Vector3D &wo,
        const std::vector<Shape*> &objList,
        const std::vector<LightSource*> &lsList
    ) const;

    Vector3D indirectRadiance(
        const Intersection &its,
        const Vector3D &wo,
        int depth,
        const std::vector<Shape*> &objList,
        const std::vector<LightSource*> &lsList
    ) const;
};

#endif

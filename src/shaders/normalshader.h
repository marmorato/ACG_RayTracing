//
//  normalshader.h
//  ACG
//
//  Created by mar morató aguilar on 13/10/26.
//

#ifndef NORMALSHADER_H
#define NORMALSHADER_H

#include "shader.h"

class NormalShader : public Shader
{
public:
    NormalShader();

    virtual Vector3D computeColor(
        const Ray &r,
        const std::vector<Shape*> &objList,
        const std::vector<LightSource*> &lsList
    ) const;
};

#endif // NORMALSHADER_H


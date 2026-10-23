
#ifndef TRANSMISSIVEMATERIAL_H
#define TRANSMISSIVEMATERIAL_H

#include "material.h"

class Transmissive : public Material
{
public:
    Transmissive(double mu);

    Vector3D getReflectance(
        const Vector3D& n,
        const Vector3D& wo,
        const Vector3D& wi
    ) const;

    bool hasSpecular() const { return false; }
    bool hasTransmission() const { return true; }
    bool hasDiffuseOrGlossy() const { return false; }
    bool isEmissive() const { return false; }

    double getIndexOfRefraction() const;
    Vector3D getEmissiveRadiance() const;
    Vector3D getDiffuseReflectance() const;

private:
    double mu_t;
};

#endif

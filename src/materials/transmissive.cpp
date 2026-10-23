
#include "transmissive.h"

Transmissive::Transmissive(double mu): mu_t(mu){}

Vector3D Transmissive::getReflectance(
    const Vector3D& n,
    const Vector3D& wo,
    const Vector3D& wi
) const
{
    return Vector3D(0.0);
}

double Transmissive::getIndexOfRefraction() const
{
    return mu_t;
}

Vector3D Transmissive::getEmissiveRadiance() const
{
    return Vector3D(0.0);
}

Vector3D Transmissive::getDiffuseReflectance() const
{
    return Vector3D(0.0);
}

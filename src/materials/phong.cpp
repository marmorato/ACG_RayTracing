
#include "phong.h"

#include <iostream>
#include <cmath>

Phong::Phong()
{ }

Phong::Phong(Vector3D Kd_, Vector3D Ks_, float alpha_):
rho_d(Kd_), Ks(Ks_), alpha(alpha_){}


Vector3D Phong::getReflectance(const Vector3D& n, const Vector3D& wo,
    const Vector3D& wi) const {

    //FILL(...)
    // Compute the ideal reflection direction
        Vector3D wr = 2.0 * dot(n, wi) * n - wi;
        double cosAlpha = std::max(dot(wo, wr), 0.0);


        // Diffuse component
        Vector3D diffuse = rho_d / M_PI;

        // Specular component
        double specular = pow(cosAlpha, alpha);
        double normalization = (alpha +2.0)/(2.0*M_PI);
        Vector3D specularColor = Ks * specular * normalization;

        // Phong reflectance
        return diffuse + specularColor;

};

double Phong::getIndexOfRefraction() const
{
    std::cout << "Warning! Calling \"Material::getIndexOfRefraction()\" for a non-transmissive material"
              << std::endl;

    return -1;
}


Vector3D Phong::getEmissiveRadiance() const
{
    return Vector3D(0.0);
}


Vector3D Phong::getDiffuseReflectance() const
{
    return rho_d;
}

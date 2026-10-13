#include "phong.h"

#include <iostream>

Phong::Phong()
{ }

Phong::Phong(Vector3D Kd_, Vector3D Ks_, float alpha_):
rho_d(Kd_), Ks(Ks_), alpha(alpha_){}


Vector3D Phong::getReflectance(const Vector3D& n, const Vector3D& wo,
    const Vector3D& wi) const {

    //FILL(...)
    // Compute the ideal reflection direction
        Vector3D wr = 2.0 * (n * wi) * n - wi;

        // Diffuse component
        Vector3D diffuse = rho_d / M_PI;

        // Specular component
        double dotProduct = wo.x * wr.x + wo.y * wr.y + wo.z * wr.z;
        double specular = pow(dotProduct, alpha);
        Vector3D specularColor = Ks * specular;

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


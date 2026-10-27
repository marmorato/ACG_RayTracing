#include "arealightsource.h"
#include "../core/hemisphericalsampler.h"

AreaLightSource::AreaLightSource(Square* areaLightsource_) :
    myAreaLightsource(areaLightsource_)
{ }



Vector3D AreaLightSource::getIntensity() const
{
    return myAreaLightsource->getMaterial().getEmissiveRadiance();
}


Vector3D AreaLightSource::sampleLightPosition()   const
{
    //FILL(...)
    // 
    //New Randam Pos inside Area Lightsource
    double u = HemisphericalSampler::myRandomDouble();
        double v = HemisphericalSampler::myRandomDouble();

        return myAreaLightsource->corner
             + u * myAreaLightsource->v1
             + v * myAreaLightsource->v2;}


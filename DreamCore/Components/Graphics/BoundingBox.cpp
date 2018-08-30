#include "BoundingBox.h"

#include <limits>

namespace Dream
{
    BoundingBox::BoundingBox(vec3 min, vec3 max)
        : maxDimension(max.x*2),
          minimum(min),
          maximum(max)
    {
        //maximum = vec3(std::numeric_limits<float>::min());
        //minimum = vec3(std::numeric_limits<float>::max());
    }

    vec3 BoundingBox::getCenter()
    {
       return vec3((minimum.x+maximum.x)/2, (minimum.y+maximum.y)/2, (minimum.z+maximum.z)/2);
    }
}

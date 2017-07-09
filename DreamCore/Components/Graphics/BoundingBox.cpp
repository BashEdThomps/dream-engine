#include "BoundingBox.h"

namespace Dream
{
    BoundingBox::BoundingBox(vec3 min, vec3 max)
        : maxDimension(max.x*2)
    {
        minimum = min;
        maximum = max;
    }
}

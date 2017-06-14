#include "BoundingBox.h"

namespace Dream
{
    BoundingBox::BoundingBox(vec3 min, vec3 max)
    {
        minimum = min;
        maximum = max;
    }
}

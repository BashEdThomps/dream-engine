/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "BoundingBox.h"
#include <limits>

using std::numeric_limits;

namespace Dream
{
    BoundingBox::BoundingBox(vec3 min, vec3 max)
        : DreamObject("BoundingBox"),
          maxDimension(max.x*2),
          minimum(min),
          maximum(max)
    {
    }

    BoundingBox::~BoundingBox
    ()
    {

    }

    vec3
    BoundingBox::getCenter
    ()
    {
       return vec3((minimum.x+maximum.x)/2, (minimum.y+maximum.y)/2, (minimum.z+maximum.z)/2);
    }

    void
    BoundingBox::setToLimits
    ()
    {
        maximum = vec3(numeric_limits<float>::min());
        minimum = vec3(numeric_limits<float>::max());
    }

    vec3
    BoundingBox::getPositiveVertex
    (const vec3& position, const vec3& normal)
    const
    {
        vec3 positiveVertex = minimum;
        if( normal.x >= 0.0f ) positiveVertex.x = maximum.x;
        if( normal.y >= 0.0f ) positiveVertex.y = maximum.y;
        if( normal.z >= 0.0f ) positiveVertex.z = maximum.z;
        return position+positiveVertex;
    }

    vec3
    BoundingBox::getNegativeVertex
    (const vec3& position, const vec3& normal)
    const
    {
        vec3 negativeVertex = maximum;
        if( normal.x >= 0.0f ) negativeVertex.x = minimum.x;
        if( normal.y >= 0.0f ) negativeVertex.y = minimum.y;
        if( normal.z >= 0.0f ) negativeVertex.z = minimum.z;
        return position+negativeVertex;
    }

    void
    BoundingBox::integrate
    (const BoundingBox& bb)
    {
        // Maximum
        if (maximum.x < bb.maximum.x)
        {
            maximum.x = bb.maximum.x;
        }

        if (bb.maximum.y < bb.maximum.y)
        {
            maximum.y = bb.maximum.y;
        }

        if (bb.maximum.z < bb.maximum.z)
        {
            maximum.z = bb.maximum.z;
        }

        // Maximum
        if (bb.minimum.x > bb.minimum.x)
        {
            minimum.x = bb.minimum.x;
        }

        if (bb.minimum.y > bb.minimum.y)
        {
            minimum.y = bb.minimum.y;
        }

        if (bb.minimum.z > bb.minimum.z)
        {
            minimum.z = bb.minimum.z;
        }

        float maxBound;
        maxBound = (maximum.x > maximum.y ? maximum.x : maximum.y);
        maxBound = (maxBound > maximum.z ? maxBound : maximum.z);

        maxDimension = maxBound;
    }
}

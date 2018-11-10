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
}

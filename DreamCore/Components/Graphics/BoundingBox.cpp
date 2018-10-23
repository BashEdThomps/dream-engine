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
        init();
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
    BoundingBox::init
    ()
    {
        maximum = vec3(numeric_limits<float>::min());
        minimum = vec3(numeric_limits<float>::max());
    }

    void
    BoundingBox::updateFromMesh
    (aiMesh* mesh)
    {
        auto log = getLog();
        log->debug( "Updating bounding box");
        /*

        for (unsigned int i=0; i < mesh->mNumVertices; i++)
        {
            aiVector3D vertex = mesh->mVertices[i];

            // Maximum
            if (maximum.x < vertex.x)
            {
                maximum.x = vertex.x;
            }

            if (maximum.y < vertex.y)
            {
                maximum.y = vertex.y;
            }

            if (maximum.z < vertex.z)
            {
                maximum.z = vertex.z;
            }

            // Maximum
            if (minimum.x > vertex.x)
            {
                minimum.x = vertex.x;
            }

            if (minimum.y > vertex.y)
            {
                minimum.y = vertex.y;
            }

            if (minimum.z > vertex.z)
            {
                minimum.z = vertex.z;
            }
        }

        float maxBound;
        maxBound = (
                    maximum.x > maximum.y ?
                        maximum.x :
                        maximum.y
                        );

        maxBound = (
                    maxBound > maximum.z ?
                        maxBound :
                        maximum.z
                        );

        maxDimension = maxBound;
        */
    }
}

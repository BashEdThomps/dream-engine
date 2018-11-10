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


#pragma once
#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp>
#include "../../Common/DreamObject.h"

using namespace glm;
struct aiMesh;

namespace Dream
{
    class BoundingBox : DreamObject
    {
    public:
        BoundingBox(glm::vec3 minimum = {-0.5f,-0.5f,-0.5f}, vec3 maximum = {0.5f,0.5f,0.5f});
        ~BoundingBox();

        float maxDimension;
        void setToLimits();
        vec3 minimum;
        vec3 maximum;
        vec3 getCenter();
    };
}



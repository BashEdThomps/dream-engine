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

#include "Math/Vector.h"


namespace octronic::dream
{
    class BoundingBox
    {
    public:
        BoundingBox(const vec3& minimum = vec3(std::numeric_limits<float>::max()),
                    const vec3& maximum = vec3(std::numeric_limits<float>::min()));

        BoundingBox(const BoundingBox& other);

        ~BoundingBox();

        void operator=(const BoundingBox& bb);
        void integrate(const BoundingBox& bb);

        vec3 getNegativeVertex(const vec3& position, const vec3& normal) const;
        vec3 getPositiveVertex(const vec3& position, const vec3& normal) const;
        void setToLimits();
        vec3 getCenter() const;
        vec3 getMaximum() const;
        vec3 getMinimum() const;
        void setMinimum(const vec3& v);
        void setMaximum(const vec3& v);
        void setMaxDimension(float md);

    private:
        float mMaxDimension;
        vec3 mMinimum;
        vec3 mMaximum;
    };
}



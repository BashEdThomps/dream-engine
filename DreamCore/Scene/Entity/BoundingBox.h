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

#include "Common/Vector.h"
#include "Common/LockableObject.h"

namespace octronic::dream
{
    class BoundingBox : public LockableObject
    {
    public:
        BoundingBox(
                const Vector3& minimum = Vector3(-0.5f,-0.5f,-0.5f),
                const Vector3& maximum = Vector3(0.5f,0.5f,0.5f)
                );

        BoundingBox(const BoundingBox& other);

        ~BoundingBox();

        void from(const BoundingBox& bb);
        void integrate(const BoundingBox& bb);

        Vector3 getNegativeVertex(const Vector3& position, const Vector3& normal) const;
        Vector3 getPositiveVertex(const Vector3& position, const Vector3& normal) const;
        void setToLimits();
        Vector3 getCenter() const;
        Vector3& getMaximum();
        Vector3& getMinimum();
        void setMaxDimension(float md);
    private:
        float maxDimension;
        Vector3 minimum;
        Vector3 maximum;
    };
}



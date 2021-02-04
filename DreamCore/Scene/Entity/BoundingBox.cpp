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


namespace octronic::dream
{
    BoundingBox::BoundingBox(const Vector3& min, const Vector3& max)
        : LockableObject("BoundingBox"),
          maxDimension(max.x()*2),
          minimum(min),
          maximum(max)
    {
    }


    BoundingBox::BoundingBox(const BoundingBox& other)
        : LockableObject("BoundingBox"),
          maxDimension(other.maxDimension),
          minimum(other.minimum),
          maximum(other.maximum)
    {

    }

    BoundingBox::~BoundingBox
    ()
    {

    }

    void BoundingBox::from(const BoundingBox &bb)
    {
       maxDimension = bb.maxDimension;
       minimum = bb.minimum;
       maximum = bb.maximum;
    }

    Vector3
    BoundingBox::getCenter
    () const
    {
       return Vector3((minimum.x()+maximum.x())/2, (minimum.y()+maximum.y())/2, (minimum.z()+maximum.z())/2);
    }

    void
    BoundingBox::setToLimits
    ()
    {
        if(dreamTryLock()) {
        dreamLock();

        maximum = Vector3(numeric_limits<float>::min());
        minimum = Vector3(numeric_limits<float>::max());
        } dreamElseLockFailed
    }

    Vector3
    BoundingBox::getPositiveVertex
    (const Vector3& position, const Vector3& normal)
    const
    {
        Vector3 positiveVertex = minimum;
        if( normal.x() >= 0.0f ) positiveVertex.setX(maximum.x());
        if( normal.y() >= 0.0f ) positiveVertex.setY(maximum.y());
        if( normal.z() >= 0.0f ) positiveVertex.setZ(maximum.z());
        return position+positiveVertex;
    }

    Vector3
    BoundingBox::getNegativeVertex
    (const Vector3& position, const Vector3& normal)
    const
    {
        Vector3 negativeVertex = maximum;
        if( normal.x() >= 0.0f ) negativeVertex.setX(minimum.x());
        if( normal.y() >= 0.0f ) negativeVertex.setY(minimum.y());
        if( normal.z() >= 0.0f ) negativeVertex.setZ(minimum.z());
        return position+negativeVertex;
    }

    void
    BoundingBox::integrate
    (const BoundingBox& bb)
    {
        if(dreamTryLock()) {
        dreamLock();

        // Maximum
        if (maximum.x() < bb.maximum.x())
        {
            maximum.setX(bb.maximum.x());
        }

        if (bb.maximum.y() < bb.maximum.y())
        {
            maximum.setY(bb.maximum.y());
        }

        if (bb.maximum.z() < bb.maximum.z())
        {
            maximum.setZ(bb.maximum.z());
        }

        // Maximum
        if (bb.minimum.x() > bb.minimum.x())
        {
            minimum.setX(bb.minimum.x());
        }

        if (bb.minimum.y() > bb.minimum.y())
        {
            minimum.setY(bb.minimum.y());
        }

        if (bb.minimum.z() > bb.minimum.z())
        {
            minimum.setZ(bb.minimum.z());
        }

        float maxBound;
        maxBound = (maximum.x() > maximum.y() ? maximum.x() : maximum.y());
        maxBound = (maxBound > maximum.z() ? maxBound : maximum.z());

        maxDimension = maxBound;
        } dreamElseLockFailed
    }

    Vector3& BoundingBox::getMaximum()
    {
        return maximum;
    }

    Vector3& BoundingBox::getMinimum()
    {
        return minimum;
    }

    void BoundingBox::setMaxDimension(float md)
	{
		maxDimension = md;
	}
}

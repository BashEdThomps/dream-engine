#include "BoundingBox.h"

#include <limits>

using std::numeric_limits;

namespace octronic::dream
{
    BoundingBox::BoundingBox
    (const vec3& min, const vec3& max)
        : mMaxDimension(0),
          mMinimum(min),
          mMaximum(max)
    {
    }


    BoundingBox::BoundingBox
    (const BoundingBox& other)
        : mMaxDimension(other.mMaxDimension),
          mMinimum(other.mMinimum),
          mMaximum(other.mMaximum)
    {

    }

    void
    BoundingBox::operator=
    (const BoundingBox &bb)
    {
       mMaxDimension = bb.mMaxDimension;
       mMinimum = bb.mMinimum;
       mMaximum = bb.mMaximum;
    }

    vec3
    BoundingBox::getCenter
    ()
    const
    {
       return vec3((mMinimum.x+mMaximum.x)/2, (mMinimum.y+mMaximum.y)/2, (mMinimum.z+mMaximum.z)/2);
    }

    void
    BoundingBox::setToLimits
    ()
    {
        mMaximum = vec3(numeric_limits<float>::min());
        mMinimum = vec3(numeric_limits<float>::max());
    }

    vec3
    BoundingBox::getPositiveVertex
    (const vec3& position, const vec3& normal)
    const
    {
        vec3 positiveVertex = mMinimum;
        if( normal.x >= 0.0f ) positiveVertex.x = mMaximum.x;
        if( normal.y >= 0.0f ) positiveVertex.y = mMaximum.y;
        if( normal.z >= 0.0f ) positiveVertex.z = mMaximum.z;
        return position+positiveVertex;
    }

    vec3
    BoundingBox::getNegativeVertex
    (const vec3& position, const vec3& normal)
    const
    {
        vec3 negativeVertex = mMaximum;
        if(normal.x >= 0.0f ) negativeVertex.x = mMinimum.x;
        if(normal.y >= 0.0f ) negativeVertex.y = mMinimum.y;
        if(normal.z >= 0.0f ) negativeVertex.z = mMinimum.z;
        return position+negativeVertex;
    }

    void
    BoundingBox::integrate
    (const BoundingBox& bb)
    {
        // Maximum
        if (bb.mMaximum.x > mMaximum.x) mMaximum.x = bb.mMaximum.x;
        if (bb.mMaximum.y > mMaximum.y) mMaximum.y = bb.mMaximum.y;
        if (bb.mMaximum.z > mMaximum.z) mMaximum.z = bb.mMaximum.z;
        // Minimum
        if (bb.mMinimum.x < mMinimum.x) mMinimum.x = bb.mMinimum.x;
        if (bb.mMinimum.y < mMinimum.y) mMinimum.y = bb.mMinimum.y;
        if (bb.mMinimum.z < mMinimum.z) mMinimum.z = bb.mMinimum.z;

        float maxBound;
        maxBound = mMaximum.x > mMaximum.y ? mMaximum.x : mMaximum.y;
        maxBound = maxBound > mMaximum.z ? maxBound : mMaximum.z;
        mMaxDimension = maxBound;
    }

    vec3
    BoundingBox::getMaximum
    ()
    const
    {
        return mMaximum;
    }

    vec3
    BoundingBox::getMinimum
    ()
    const
    {
        return mMinimum;
    }

    void
    BoundingBox::setMinimum
    (const vec3 &v)
    {
        mMinimum = v;
    }

    void
    BoundingBox::setMaximum
    (const vec3 &v)
    {
        mMaximum = v;
    }

    void
    BoundingBox::setMaxDimension
    (float md)
	{
		mMaxDimension = md;
	}
}

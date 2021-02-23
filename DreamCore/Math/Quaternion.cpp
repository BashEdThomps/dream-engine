#include "Quaternion.h"

#include <cstring>

#include "Common/Constants.h"

/*
 * using
 * http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
*/

namespace octronic::dream
{
    json Quaternion::toJson(const quat& q)
    {
        json js;
        js[Constants::W] = q.w;
        js[Constants::X] = q.x;
        js[Constants::Y] = q.y;
        js[Constants::Z] = q.z;
        return js;
    }

    quat Quaternion::fromJson(const json& js)
    {
        quat q;
        if (js.find(Constants::W) != js.end()) q.w = js[Constants::W];
        if (js.find(Constants::X) != js.end()) q.x = js[Constants::X];
        if (js.find(Constants::Y) != js.end()) q.y = js[Constants::Y];
        if (js.find(Constants::Z) != js.end()) q.z = js[Constants::Z];
        return q;
    }

    btQuaternion Quaternion::toBullet(const quat& q)
    {
        return btQuaternion(q.x,q.y,q.z,q.w);
    }

    quat Quaternion::fromBullet(const btQuaternion& q)
    {
        quat qu;
        qu.w = q.w();
        qu.x = q.x();
        qu.y = q.y();
        qu.z = q.z();
        return qu;
    }
}

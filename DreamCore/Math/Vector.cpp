#include "Vector.h"

#include "Common/Constants.h"

namespace octronic::dream
{
    // Vector2 =================================================================

    vec2 Vector2::fromJson(const json& js)
    {
        vec2 v;
        if(js.find(Constants::X) != js.end()) v.x = js[Constants::X];
        if(js.find(Constants::Y) != js.end()) v.y = js[Constants::Y];
        return v;
    }

    json Vector2::toJson(const vec2& v)
    {
        json js = json::object();
        js[Constants::X] = v.x;
        js[Constants::Y] = v.y;
        return js;
    }

    // Vector3 =================================================================

    vec3 Vector3::fromBullet(const btVector3& v)
    {
		return vec3(v.x(),v.y(),v.z());
    }

    vec3 Vector3::fromJson(const json& js)
    {
        vec3 retval;
        if(js.find(Constants::X) != js.end()) retval.x = js[Constants::X];
        if(js.find(Constants::Y) != js.end()) retval.y = js[Constants::Y];
        if(js.find(Constants::Z) != js.end()) retval.z = js[Constants::Z];
        return retval;
    }

    btVector3 Vector3::toBullet(const vec3& v)
    {
        return btVector3(v.x, v.y, v.z);
    }

    json Vector3::toJson(const vec3& v)
    {
        json js = json::object();
        js[Constants::X] = v.x;
        js[Constants::Y] = v.y;
        js[Constants::Z] = v.z;
        return js;
    }

    // Vector4 =================================================================

    vec4 Vector4::fromJson(const json& js)
    {
        vec4 v;
        if(js.find(Constants::X) != js.end()) v.x = js[Constants::X];
        if(js.find(Constants::Y) != js.end()) v.y = js[Constants::Y];
        if(js.find(Constants::Z) != js.end()) v.z = js[Constants::Z];
        if(js.find(Constants::W) != js.end()) v.w = js[Constants::W];
        return v;
    }

    json Vector4::toJson(const vec4& v)
    {
        json js = json::object();
        js[Constants::W] = v.w;
        js[Constants::X] = v.x;
        js[Constants::Y] = v.y;
        js[Constants::Z] = v.z;
        return js;
    }
}

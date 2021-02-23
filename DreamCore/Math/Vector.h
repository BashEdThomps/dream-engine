#pragma once

#include <LinearMath/btVector3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <json.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using nlohmann::json;

namespace octronic::dream
{
	class Vector2 final
    {
        Vector2() = delete;
    public:
        static json toJson(const vec2& v);
        static vec2 fromJson(const json& js);
    };

    class Vector3 final
    {
        Vector3() = delete;
    public:
        static json toJson(const vec3& v);
        static vec3 fromJson(const json& js);
        static btVector3 toBullet(const vec3& v);
        static vec3 fromBullet(const btVector3& v);
    };

    class Vector4 final
    {
        Vector4() = delete;
    public:
        static vec4 fromJson(const json& js);
        static json toJson(const vec4& v);
    };
}

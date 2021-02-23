#pragma once

#include <json.hpp>
#include <glm/gtc/quaternion.hpp>
#include <LinearMath/btQuaternion.h>

using nlohmann::json;
using glm::quat;

namespace octronic::dream
{
	class Quaternion final
	{
        Quaternion() = delete;
	public:
        static quat fromJson(const json& js);
		static json toJson(const quat&);
        static quat fromBullet(const btQuaternion& q);
		static btQuaternion toBullet(const quat&);
	};
}

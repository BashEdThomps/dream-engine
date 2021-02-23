#pragma once

#include <glm/matrix.hpp>
#include <json.hpp>

using glm::mat4;
using nlohmann::json;

namespace octronic::dream
{
	class Matrix final
	{
		Matrix() = delete;
	public:
		static mat4 fromJson(const json& js);
		static json toJson(const mat4& js);
	};
}

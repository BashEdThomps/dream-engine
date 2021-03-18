#pragma once

#include <glm/vec3.hpp>
using glm::vec3;

namespace octronic::dream
{
	struct PathControlPoint
    {
       int id = 0;
       int index = 0;
       vec3 position = vec3(0.0f);
    };
}

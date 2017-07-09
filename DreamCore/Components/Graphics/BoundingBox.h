#pragma once

#include <glm/vec3.hpp>

using namespace glm;

namespace Dream
{
    class BoundingBox
    {
    public:
        BoundingBox(glm::vec3 minimum = {-1.0f,-1.0f,-1.0f}, vec3 maximum = {1.0f,1.0f,1.0f});
        float maxDimension;
        vec3 minimum;
        vec3 maximum;
    };
}



#pragma once

#include <vector>
#include <glm/mat4x4.hpp>

using std::vector;
using glm::mat4;

namespace octronic::dream
{
    struct VertexWeight
    {
        unsigned int id;
        float weight;
    };

    struct Bone
    {
       vector<VertexWeight> weights;
       mat4 offsetMatrix;
    };
}

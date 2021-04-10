#pragma once

#include "Common/Constants.h"
#include <iostream>
#include <math.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <LinearMath/btVector3.h>
#include <json.hpp>

using nlohmann::json;


namespace octronic::dream
{
    class Conversion
    {
    public:
        static float degreesToRadians(float degrees)
        {
            return static_cast<float>(degrees*(M_PI/180));
        }

        static float radiansToDegrees(float radians)
        {
            return static_cast<float>(radians*(180/M_PI));
        }
    };
}

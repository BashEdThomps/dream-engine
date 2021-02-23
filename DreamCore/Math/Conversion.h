/*
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

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

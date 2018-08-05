/*
 * Math.cpp
 *
 * Created: 05/04/2017 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
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
#include "Math.h"

#define _USE_MATH_DEFINES
#include <cmath>

namespace Dream
{
    Math::Math()  {}
    Math::~Math() {}

    float Math::degreesToRadians(float degrees)
    {
        return static_cast<float>(degrees*(M_PI/180));
    }

    float Math::radiansToDegrees(float radians)
    {
        return static_cast<float>(radians*(180/M_PI));
    }

    float Math::_sinf(float x)
    {
        return sinf(x);
    }

    float Math::_pow(float x, float y)
    {
        return pow(x,y);
    }

    float Math::_sqrtf(float x)
    {
        return sqrtf(x);
    }

} // End of Dream

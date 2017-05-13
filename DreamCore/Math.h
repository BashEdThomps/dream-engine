/*
 * Math.h
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
#ifndef MATH_H
#define MATH_H

#include "Lua/ILuaExposable.h"

namespace Dream
{
    class Math : public ILuaExposable
    {
        Math();
        ~Math();
        float degreesToRadians(float degrees);
        float radiansToDegrees(float radians);
        void exposeLuaApi(lua_State*);
    }; // End of Math
} // End of Dream

#endif // MATH_H

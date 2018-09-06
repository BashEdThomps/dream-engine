/*
 * ShaderCache.h
 *
 * Created: 16/12/2016 2016 by Ashley
 *
 * Copyright 2016 Octronic. All rights reserved.
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

#pragma once

#include <iostream>
#include <map>
#include <GL/glew.h>

#include "../../../Common/Constants.h"
#include "../../../Common/DreamObject.h"

using namespace std;

namespace Dream
{
    class ShaderCache : DreamObject
    {

    private:
        map<string,GLuint> mCache;
    public:
        ShaderCache();
        ~ShaderCache();

        GLuint getShader(string);
        void   putShader(string,GLuint);
    };
}

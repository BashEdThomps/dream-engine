/*
 * ShaderCache
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
#include "ShaderCache.h"

namespace Dream
{

    map<string,GLuint> ShaderCache::sShaderCache = map<string,GLuint>();

    GLuint
    ShaderCache::getShader
    (string uuid)
    {
        map<string,GLuint>::iterator it;
        for (it=sShaderCache.begin();it!=sShaderCache.end();it++)
        {
            if ((*it).first.compare(uuid) == 0)
            {
                if (DEBUG)
                {
                    cout << "ShaderCache: Found Shader " << uuid << endl;
                }
                return (*it).second;
            }
        }
        return 0;
    }

    void
    ShaderCache::putShader
    (string uuid, GLuint shaderProgram)
    {
        sShaderCache.insert(pair<string,GLuint>(uuid,shaderProgram));
    }

} // End of Dream

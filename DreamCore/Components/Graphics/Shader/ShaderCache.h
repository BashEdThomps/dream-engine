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

#include <glm/mat4x4.hpp>
#include "../../ICache.h"

using namespace std;
using glm::vec3;
using glm::mat4;

namespace Dream
{
    class LightInstance;

    class ShaderCache : public ICache
    {
    public:
        ShaderCache(ProjectRuntime* rt);
        ~ShaderCache() override;
        void logShaders();
        void draw(mat4 viewMatrix, mat4 projectionMatrix, vec3 viewPos);

        // ICache interface
    protected:
        IAssetInstance* loadInstance(IAssetDefinition* def) override;
    };
}

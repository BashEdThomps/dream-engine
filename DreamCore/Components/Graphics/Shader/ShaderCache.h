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
#include "../../Cache.h"
#include "../Light/LightInstance.h"

using namespace std;
using namespace glm;

namespace Dream
{
    class LightInstance;
    class Camera;
    class ShaderInstance;

    class ShaderCache : public Cache
    {
    public:
        ShaderCache(ProjectRuntime* rt);
        ~ShaderCache() override;
        void logShaders();
        void drawGeometryPass(Camera*);
        void drawShadowPass(mat4, ShaderInstance*);

        // Cache interface
    protected:
        SharedAssetInstance* loadInstance(AssetDefinition* def) override;
    };
}

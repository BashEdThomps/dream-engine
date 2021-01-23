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

#include <glm/mat4x4.hpp>
#include "Components/Cache.h"
#include "Components/Graphics/Light/LightRuntime.h"

using namespace std;
using namespace glm;

namespace octronic::dream
{
    class LightRuntime;
    class Camera;
    class ShaderRuntime;

    class ShaderCache : public Cache
    {
    public:
        ShaderCache(ProjectRuntime* rt);
        ~ShaderCache() override;
        void logShaders();
        void drawGeometryPass(Camera*);
        void drawShadowPass(mat4, ShaderRuntime*);

        // Cache interface
    protected:
        SharedAssetRuntime* loadRuntime(AssetDefinition* def) override;
    };
}

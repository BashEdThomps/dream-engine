/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <string>
#include <map>
#include <vector>

#include <glm/matrix.hpp>
#include "../Component.h"

using namespace std;
using namespace glm;

typedef struct NVGcontext NVGcontext;
typedef unsigned int GLuint;

namespace Dream
{
    class ShaderCache;
    class WindowComponent;
    class ModelRuntime;
    class ShaderRuntime;
    class LightRuntime;
    class SceneRuntime;
    class SceneObjectRuntime;
    class Texture;
    class ModelMesh;
    class MaterialRuntime;
    class ShaderCache;
    class ShaderRuntime;

    /**
     * @brief Responsible for managing Dream's graphics pipeline.
     */
    class GraphicsComponent
            : public Component
    {
    private:

        vector<LightRuntime*> mLightQueue;
        WindowComponent* mWindowComponent;
        ShaderCache* mShaderCache;
        // Geom
        GLuint mGeometryPassFB;
        GLuint mGeometryPassPositionBuffer;
        GLuint mGeometryPassAlbedoBuffer;
        GLuint mGeometryPassNormalBuffer;
        GLuint mGeometryPassDepthBuffer;
        GLuint mGeometryPassIgnoreBuffer;

        // Shadow
        SceneObjectRuntime* mShadowLight;
        ShaderRuntime* mShadowPassShader;
        GLuint mShadowPassFB;
        GLuint mShadowPassDepthBuffer;
        mat4 mShadowMatrix;

        // Lighting
        ShaderRuntime* mLightingPassShader;
        GLuint mScreenQuadVAO;
        GLuint mScreenQuadVBO;

        const int SHADOW_WIDTH = 1024;
        const int SHADOW_HEIGHT = 1024;

    public:
        GraphicsComponent(WindowComponent*);
        ~GraphicsComponent() override;

        bool init() override;
        void updateComponent(SceneRuntime*) override;

        void addToLightQueue(LightRuntime*);
        void clearLightQueue();
        void updateLightQueue(SceneRuntime*);
        bool setupScreenQuad();
        void renderLightingPass(SceneRuntime* sr);

        bool setupGeometryBuffers();
        void freeGeometryBuffers();
        void renderGeometryPass(SceneRuntime*);

        bool setupShadowBuffers();
        void freeShadowBuffers();
        void renderShadowPass(SceneRuntime*);

        void onWindowDimensionsChanged();
        void handleResize();

        void setShaderCache(ShaderCache* cache);

        ShaderRuntime* getLightingShader() const;
        void setLightingShader(ShaderRuntime* lightingShader);

        ShaderRuntime* getShadowPassShader() const;
        void setShadowPassShader(ShaderRuntime* shadowPassShader);

        GLuint getGeometryPassPositionBuffer() const;
        GLuint getGeometryPassAlbedoBuffer() const;
        GLuint getGeometryPassNormalBuffer() const;
        GLuint getGeometryPassDepthBuffer() const;
        GLuint getShadowPassDepthBuffer() const;
        GLuint getGeometryPassIgnoreBuffer() const;
    };
}

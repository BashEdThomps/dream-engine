/*
* GraphicsComponent
*
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
    class IWindowComponent;
    class ModelInstance;
    class ShaderInstance;
    class LightInstance;
    class SceneRuntime;
    class SceneObjectRuntime;
    class Texture;
    class ModelMesh;
    class MaterialInstance;
    class ShaderCache;
    class ShaderInstance;

    class GraphicsComponent : public Component
    {
    private:
        mat4 mProjectionMatrix;
        float mMinimumDraw;
        float mMaximumDraw;
        float mMeshCullDistance;
        vector<LightInstance*> mLightQueue;
        IWindowComponent* mWindowComponent;
        ShaderCache* mShaderCacheHandle;
        // Geom
        GLuint mGeometryPassFB;
        GLuint mGeometryPassPositionBuffer;
        GLuint mGeometryPassAlbedoBuffer;
        GLuint mGeometryPassNormalBuffer;
        GLuint mGeometryPassDepthBuffer;
        GLuint mGeometryPassIgnoreBuffer;

        // Shadow
        SceneObjectRuntime* mShadowLight;
        ShaderInstance* mShadowPassShader;
        GLuint mShadowPassFB;
        GLuint mShadowPassDepthBuffer;
        mat4 mShadowMatrix;

        // Lighting
        ShaderInstance* mLightingPassShader;
        GLuint mScreenQuadVAO;
        GLuint mScreenQuadVBO;

        const int SHADOW_WIDTH = 2048;
        const int SHADOW_HEIGHT = 2048;

    public:
        GraphicsComponent(IWindowComponent*);
        ~GraphicsComponent() override;

        bool init() override;
        void updateComponent(SceneRuntime*) override;

        void addToLightQueue(LightInstance*);
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

        mat4 getProjectionMatrix();
        void onWindowDimensionsChanged();
        void handleResize();
        float getMeshCullDistance() const;
        void setMeshCullDistance(float meshCullDistance);
        void setMinimumDraw(float minimumDraw);
        void setMaximumDraw(float maximumDraw);
        void setShaderCache(ShaderCache* cache);
        ShaderInstance* getLightingShader() const;
        void setLightingShader(ShaderInstance* lightingShader);

        float getMinimumDraw() const;
        float getMaximumDraw() const;
        ShaderInstance* getShadowPassShader() const;
        void setShadowPassShader(ShaderInstance* shadowPassShader);
        GLuint getGeometryPassPositionBuffer() const;
        GLuint getGeometryPassAlbedoBuffer() const;
        void setGeometryPassAlbedoBuffer(const GLuint& geometryPassAlbedoBuffer);
        GLuint getGeometryPassNormalBuffer() const;
        GLuint getGeometryPassDepthBuffer() const;
        GLuint getShadowPassDepthBuffer() const;
        GLuint getGeometryPassIgnoreBuffer() const;
    }; // End of GraphicsComponent
} // End of Dream

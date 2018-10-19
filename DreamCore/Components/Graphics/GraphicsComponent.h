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

#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#include <string>
#include <map>
#include <vector>

#include <glm/matrix.hpp>
#include "../IComponent.h"

using std::string;
using std::map;
using std::vector;
using glm::mat4;

typedef struct NVGcontext NVGcontext;

namespace Dream
{
    class ShaderCache;
    class IWindowComponent;
    class Camera;
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

    class GraphicsComponent : public IComponent
    {
    private:
        mat4 mProjectionMatrix;
        mat4 mOrthoProjection;
        mat4 mViewMatrix;

        Camera* mCamera;

        float mMinimumDraw;
        float mMaximumDraw;
        float mMeshCullDistance;

        vector<LightInstance*> mLightQueue;

        IWindowComponent* mWindowComponent;
        ShaderCache* mShaderCacheHandle;

        GLuint mGeometryPassFB;
        GLuint mGeometryPassAlbedoBuffer;
        GLuint mGeometryPassPositionBuffer;
        GLuint mGeometryPassNormalBuffer;
        GLuint mGeometryPassDepthBuffer;

        GLuint mScreenQuadVAO;
        GLuint mScreenQuadVBO;
        ShaderInstance* mLightingShader;

    public:
        GraphicsComponent(Camera*, IWindowComponent*);
        ~GraphicsComponent() override;

        bool init(void) override;
        void updateComponent() override;

        void addToLightQueue(LightInstance*);
        void clearLightQueue();
        void updateLightQueue();
        void setupScreenQuad();
        void renderLightingPass();


        void setupGeometryBuffers();
        void renderGeometryPass();

        void setWindowShouldClose(bool);
        Camera* getCamera();
        mat4 getViewMatrix();
        mat4 getProjectionMatrix();
        void onWindowDimensionsChanged();
        void handleResize();
        float getMeshCullDistance() const;
        void setMeshCullDistance(float meshCullDistance);
        void setMinimumDraw(float minimumDraw);
        void setMaximumDraw(float maximumDraw);
        bool getUpdateInstanceMapFlag() const;
        void setUpdateInstanceMapFlag(bool updateInstanceMapFlag);
        void setShaderCache(ShaderCache* cache);
        ShaderInstance* getLightingShader() const;
        void setLightingShader(ShaderInstance* lightingShader);

    }; // End of GraphicsComponent
} // End of Dream

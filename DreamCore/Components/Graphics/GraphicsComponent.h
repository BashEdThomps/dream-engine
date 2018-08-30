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

#include <GL/glew.h>
#include <glm/matrix.hpp>
#include "../IComponent.h"

using std::string;
using std::map;
using std::vector;
using glm::mat4;

typedef struct NVGcontext NVGcontext;

namespace Dream
{
    class IWindowComponent;
    class Camera;
    class AssimpModelInstance;
    class ShaderInstance;
    class LightInstance;
    class SceneRuntime;
    class SceneObjectRuntime;
    class Texture;
    class AssimpMesh;
    class AssimpMaterial;

    enum ModelQueueType
    {
        OPTIMISED,
        LINEAR
    };

    class GraphicsComponent : public IComponent
    {
    private:
        mat4 mProjectionMatrix;
        mat4 mViewMatrix;
        mat4 mOrthoProjection;

        GLfloat mSpriteVertices[24] =
        {
            // Pos      // Tex
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
        };

        GLuint mSpriteVBO;
        GLuint mSpriteQuadVAO;

        GLuint mFontVBO;
        GLuint mFontVAO;

        Camera *mCamera;

        float mMinimumDraw;
        float mMaximumDraw;
        float mMeshCullDistance;

        vector<SceneObjectRuntime*> mSpriteQueue;
        vector<SceneObjectRuntime*> mModelQueue;
        vector<SceneObjectRuntime*> mFontQueue;
        vector<LightInstance*>      mLightQueue;
        IWindowComponent *mWindowComponentHandle;
    public:
        GraphicsComponent(Camera*,IWindowComponent*, bool parallel = false);
        ~GraphicsComponent() override;

        void clearSpriteQueue();
        void addToSpriteQueue(SceneObjectRuntime*);
        void drawSpriteQueue();

        void addToLightQueue(LightInstance*);
        void clearLightQueue();

        void clearModelQueue();
        void addToModelQueue(SceneObjectRuntime*);
        void drawModelQueue();
        void preModelRender();
        void postModelRender();

        void clearFontQueue();
        void addToFontQueue(SceneObjectRuntime*);
        void drawFontQueue();
        void preFontRender();
        void postFontRender();

        bool init(void) override;
        void updateComponent() override;
        void drawSprite(SceneObjectRuntime*);
        void drawFont(SceneObjectRuntime*);
        void drawModel(SceneObjectRuntime*);
        void create2DVertexObjects();
        void createFontVertexObjects();
        void setWindowShouldClose(bool);
        Camera* getCamera();
        mat4 getViewMatrix();
        mat4 getProjectionMatrix();
        void onWindowDimensionsChanged();
        void handleResize();
        float getMeshCullDistance() const;
        void setMeshCullDistance(float meshCullDistance);
        ModelQueueType mModelQueueType;
        void debugOptimisedModelQueue();
        void setMinimumDraw(float minimumDraw);
        void setMaximumDraw(float maximumDraw);
    }; // End of GraphicsComponent
} // End of Dream

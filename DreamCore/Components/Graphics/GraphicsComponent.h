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

namespace Dream
{
    class IWindowComponent;
    class SceneObject;
    class Camera;
    class AssimpModelInstance;
    class ShaderInstance;
    class LightInstance;
    class Scene;
    class JsonData;

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

        float mMinimumDraw = 0.1f;
        float mMaximumDraw = 1000.0f;

        vector<SceneObject*> m2DQueue;
        vector<SceneObject*> m3DQueue;
        vector<LightInstance*> mLightQueue;

        IWindowComponent *mWindowComponentHandle;
        Scene* mActiveSceneHandle;
    public:
        GraphicsComponent(Camera*,IWindowComponent*);
        ~GraphicsComponent(void);
        void clear2DQueue();
        void addTo2DQueue(SceneObject*);
        void draw2DQueue();
        void addToLightQueue(LightInstance*);
        void clearLightQueue();
        void clear3DQueue();
        void addTo3DQueue(SceneObject*);
        void draw3DQueue();
        bool init(void) override;
        void updateComponent(Scene*) override;
        void drawSprite(SceneObject*);
        void drawFont(SceneObject*);
        void drawModel(SceneObject*);
        void create2DVertexObjects();
        void createFontVertexObjects();
        void setWindowShouldClose(bool);
        Camera* getCamera();
        mat4 getViewMatrix();
        mat4 getProjectionMatrix();
        void onWindowDimensionsChanged();
        void preRender();
        void postRender();
        void cleanUp(Scene* scene) override;
        void setActiveScene(Scene* scene);

    }; // End of GraphicsComponent

} // End of Dream

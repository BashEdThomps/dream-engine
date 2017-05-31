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

#ifndef GRAPHICSCOMPONENT_H
#define GRAPHICSCOMPONENT_H

// STD LIB
#include <iostream>
#include <map>
#include <vector>
// Dream
#include "../../Constants.h"
#include "../../GameController.h"
#include "../Window/IWindowComponent.h"
#include "../IComponent.h"
#include "AssimpModelInstance.h"
#include "ShaderInstance.h"
#include "Camera.h"
// GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
// Constants
#define CLEAR_RED   0
#define CLEAR_GREEN 1
#define CLEAR_BLUE  2
#define CLEAR_ALPHA 3

using namespace std;

namespace Dream
{

    class GraphicsComponent : public IComponent
    {
    private:
        glm::mat4 mProjectionMatrix;
        glm::mat4 mViewMatrix;

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
        float mMaximumDraw = 100.0f;
        vector<SDL_Event> mEvents;
        vector<SceneObject*> m2DQueue;
        vector<SceneObject*> m3DQueue;
        vector<LightInstance*> mLightQueue;
        glm::mat4 mOrthoProjection;
        vector<float> mClearColour;
        vector<float> mAmbientLightColour;
        GameController *mGameController;
        IWindowComponent *mWindowComponent;
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
        bool checkGLError(string);
        void setClearColour(vector<float>);
        void setAmbientLightColour(vector<float>);
        void create2DVertexObjects();
        void createFontVertexObjects();
        void setWindowShouldClose(bool);
        Camera* getCamera();
        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix();
        void setGameController(GameController* gameController);
        void onWindowDimensionsChanged();
        void preRender();
        void postRender();
        GameController *getGameController();
        void cleanUp() override;
    }; // End of GraphicsComponent
} // End of Dream
#endif // End of GRAPHICSCOMPONENT_H

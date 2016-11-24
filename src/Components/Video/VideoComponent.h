/*
* VideoComponent
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

#ifndef OpenGLVIDEO_H
#define OpenGLVIDEO_H

// Dream
#include "../ComponentInterface.h"
#include "AssimpModelInstance.h"
#include "ShaderInstance.h"
#include "Camera.h"

// STD LIB
#include <iostream>
#include <map>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

#define VIDEO_INTERFACE_DEFAULT_SCREEN_WIDTH 1280
#define VIDEO_INTERFACE_DEFAULT_SCREEN_HEIGHT 720

namespace Dream {
  namespace Components {
    namespace Video {
      class VideoComponent : public ComponentInterface {
      private:
        int mScreenWidth;
        int mScreenHeight;
        std::string mScreenName;
        bool mWindowShouldClose;
        GLFWwindow *mWindow;
        Camera *mCamera;
        float mMinimumDraw = 0.1f;
        float mMaximumDraw = 1000.0f;

      public:
        void setScreenWidth(int);
        int  getScreenWidth();
        void setScreenHeight(int);
        int  getScreenHeight();
        void setScreenName(std::string);
        std::string getScreenName();

        void closeWindow();
        void setCursorEnabled(bool);

        VideoComponent(Camera*);
        ~VideoComponent(void);

        bool init(void);
        void update(Dream::Scene*);
        void drawSceneObject(Dream::SceneObject*);
        bool checkGLError(int);
        void setupWindowEventHandlers();
        bool isWindowShouldCloseFlagSet();
        GLFWwindow* getWindow();
      }; // End of VideoComponent

    } // End of Video
  } // End of Components
} // End of Dream

#endif // End of OpenGLVIDEO_H

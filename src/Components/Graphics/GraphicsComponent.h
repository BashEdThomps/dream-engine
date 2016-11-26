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

#ifndef OpenGLGraphics_H
#define OpenGLGraphics_H

// Dream
#include "../ComponentInterface.h"
#include "AssimpModelInstance.h"
#include "ShaderInstance.h"
#include "Camera.h"

// STD LIB
#include <iostream>
#include <map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

#define Graphics_INTERFACE_DEFAULT_SCREEN_WIDTH 1280
#define Graphics_INTERFACE_DEFAULT_SCREEN_HEIGHT 720

namespace Dream {
    namespace Components {
        namespace Graphics {
            class GraphicsComponent : public ComponentInterface {
            private:
                int mWindowWidth;
                int mWindowHeight;
                std::string mScreenName;
                bool mWindowShouldClose;
                SDL_Window *mWindow;
                SDL_Renderer *mRenderer;
                Camera *mCamera;
                float mMinimumDraw = 0.1f;
                float mMaximumDraw = 1000.0f;
                SDL_GLContext mContext;
                SDL_Event mEvent;
            public:
                void setWindowWidth(int);
                int  getWindowWidth();
                void setWindowHeight(int);
                int  getWindowHeight();
                void setScreenName(std::string);
                std::string getScreenName();

                void closeWindow();
                void setCursorEnabled(bool);

                GraphicsComponent(Camera*);
                ~GraphicsComponent(void);
                bool createSDLWindow();

                bool init(void);
                void update(Dream::Scene*);
                void drawSceneObject(Dream::SceneObject*);
                bool checkGLError(int);
                void setupWindowEventHandlers();
                bool isWindowShouldCloseFlagSet();
                SDL_Window* getWindow();
                SDL_Renderer* getRenderer();
                bool createSDLRenderer();
            }; // End of GraphicsComponent

        } // End of Graphics
    } // End of Components
} // End of Dream

#endif // End of OpenGLGraphics_H

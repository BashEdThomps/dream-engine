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

// Maintain include order for GL
#ifdef WIN32
#include <Windows.h>
#endif

#include <vector>

#include <GLFW/glfw3.h>
#include "../../DreamCore/Components/Window/WindowComponent.h"

using namespace std;
using namespace Dream;

namespace DreamGLFW
{
    class GLWidget;
    class GLFWWindowComponent
        : public WindowComponent
    {
    public:
        GLFWWindowComponent();
        ~GLFWWindowComponent() override;

        GLFWwindow* getGlfwWindow();
        void updateWindow(SceneRuntime* sr) override;
        void getCurrentDimensions() override;
        void swapBuffers() override;
        bool init() override;
        void bindDefaultFrameBuffer() override;
        void drawImGui();
        void drawGLWidgets();

        static bool WindowSizeChanged;
        static bool MouseButtonsDown[5];
        static float MousePosX;
        static float MousePosY;
        static float MouseWheel;
        static float MouseWheelH;
        static bool KeysDown[512];

        static float LastTime;
        static float CurrentTime;
        static int Frames;

        static int FPS();

    private:
        bool initGLFW();
        bool initGL();
        GLFWwindow* mWindow;
        float mDPIScaleX, mDPIScaleY;
    };
}

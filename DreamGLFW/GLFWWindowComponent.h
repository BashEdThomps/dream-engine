#pragma once

// Maintain include order for GL
#ifdef WIN32
#include <Windows.h>
#endif

#include <DreamCore.h>
#include <GLFW/glfw3.h>

using octronic::dream::WindowComponent;
using octronic::dream::SceneRuntime;

namespace octronic::dream::glfw
{
    class GLWidget;

    /**
     * @brief The GLFWWindowComponent class provides a GLFW implementation of Dream.
     * This will initialise the window and OpenGL Context and provide input events
     * to the engine.
     */
    class GLFWWindowComponent : public WindowComponent
    {
    public:
        GLFWWindowComponent();
        ~GLFWWindowComponent() override;

        GLFWwindow* getGlfwWindow();
        void updateWindow() override;
        void getCurrentDimensions() override;
        void swapBuffers() override;
        bool init() override;
        void bindFrameBuffer() override;
        GLuint getFrameBuffer() const override;
        GLuint getDepthBuffer() const override;
        void drawImGui();
        void drawGLWidgets();
        void pushTasks() override;

    	// These members are static so they can be accessed by GLFW Callbacks;
        static bool WindowSizeChanged;
        static bool MouseButtonsDown[5];
        static float MousePosX;
        static float MousePosY;
        static float MouseWheel;
        static float MouseWheelH;
        static bool KeysDown[512];

    private:
        bool initGLFW();
        bool initGL();
        GLFWwindow* mWindow;
        float mDPIScaleX, mDPIScaleY;
        float mLastWidth, mLastHeight;
    };
}

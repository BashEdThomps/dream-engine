/*
 * GLFWWindowComponent.h
 *
 * Created: 11 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
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

#include <vector>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

// Maintain include order for GL
#include <GLFW/glfw3.h>
#include <imgui.h>
#ifdef IMGUI_HAS_DOCK
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#else
	#include <imgui_impl_glfw.h>
	#include <imgui_impl_opengl3.h>
#endif
#include <DreamCore.h>


namespace octronic::dream::tool
{
    class GLWidget;
    class ImGuiWidget;

    class DreamToolWindow
    {
    public:
        DreamToolWindow();
        ~DreamToolWindow();

        static ImFont* RegularFont;
        static ImFont* MonoFont;
        static float MouseWheelX;
        static float MouseWheelY;

        GLFWwindow* getGlfwWindow();
        void getCurrentDimensions();
        void swapBuffers();
        bool init();
        void bindDefaultFrameBuffer();
        void drawImGui();
        void drawGLWidgets();
        void update();

        void addImGuiWidget(ImGuiWidget& widget);
        void removeImGuiWidget(ImGuiWidget& widget);

        void addGLWidget(GLWidget& widget);
        void removeGLWidget(GLWidget& widget);

        float getUiFontSize() const;
        void setUiFontSize(float uiFontSize);

        float getMonoFontSize() const;
        void setMonoFontSize(float monoFontSize);
        bool shouldClose() const;

    private:
        bool initGLFW();
        bool initGL();
        bool initImGui();
        void cleanUpImGui();
        void setTheme();
        void setFont();
        GLFWwindow* mWindow;
        vector<reference_wrapper<GLWidget>> mGLWidgets;
        vector<reference_wrapper<ImGuiWidget>> mImGuiWidgets;
        float mUiFontSize;
        float mMonoFontSize;
        float mDPIScaleX, mDPIScaleY;
        int mWidth, mHeight;
        bool mShouldClose;
        bool mSizeHasChanged;
    }; // End of GLFWWindowComponent

} // End of Dream

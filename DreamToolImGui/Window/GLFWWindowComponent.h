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

#include <memory>
#include <vector>

#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include "../deps/ImGui/imgui.h"
#include "../deps/ImGui/imgui_impl_glfw.h"
#include "../deps/ImGui/imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <DreamCore.h>

using std::unique_ptr;
using std::vector;
using Dream::IWindowComponent;
using Dream::SceneRuntime;

namespace DreamTool
{
    class DTWidget;
}

using DreamTool::DTWidget;

namespace DreamGLFW
{

    class GLFWWindowComponent : public IWindowComponent
    {
    public:
        GLFWWindowComponent();
        ~GLFWWindowComponent() override;

        void updateComponent() override;
        void getCurrentDimensions() override;
        void swapBuffers() override;
        bool init() override;
        void bindDefaultFrameBuffer() override;
              void drawImGui();

            void addWidget(DTWidget* widget);
            void removeWidget(DTWidget* widget);

    private:
        bool initGLFW();
        bool initGL();
              bool initImGui();
              void cleanUpImGui();
        GLFWwindow* mWindow;
            vector<DTWidget*> mWidgets;

    }; // End of GLFWWindowComponent

} // End of Dream

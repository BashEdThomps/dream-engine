/*
 * GLFWWindowComponent.cpp
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

#include "DTWindowComponent.h"
#include "../Tools/DTWidget.h"

using Dream::Constants;
using Dream::SceneRuntime;
using Dream::SceneState;


static bool WindowSizeChanged = false;

namespace DreamTool
{
    void
    FramebufferSizeCallback
    (GLFWwindow* windows98, int w, int h)
    {
        WindowSizeChanged = true;
    }

    DTWindowComponent::DTWindowComponent
    () : IWindowComponent()
    {
        auto log = spdlog::get("GLFWWindowComponent");
        if (log == nullptr)
        {
            log = spdlog::stdout_color_mt("GLFWWindowComponent");
        }
        log->info("Constructing" );
        mName = "Dream";
    }

    DTWindowComponent::~DTWindowComponent
    ()
    {
        auto log = spdlog::get("GLFWWindowComponent");
        log->info("Destructing" );
        cleanUpImGui();
        glfwTerminate();
        mWindow = nullptr;
    }

    bool
    DTWindowComponent::init
    ()
    {
        if (!initGLFW())
        {
            return false;
        }

        if (!initImGui())
        {
            return false;
        }

        if (!initGL())
        {
            return false;
        }

        return true;
    }

    void
    DTWindowComponent::bindDefaultFrameBuffer
    ()
    {
        glBindFramebuffer(GL_FRAMEBUFFER,0);
    }

    bool
    DTWindowComponent::initGLFW
    ()
    {
        auto log = spdlog::get("GLFWWindowComponent");
        log->debug("Initialising GLFW");

        /* Initialize the library */
        if (!glfwInit())
        {
            log->error("FAILED @ Initialising GLFW");
            return false;
        }

        /* Create a windowed mode window and its OpenGL context */
#ifdef WIN32
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#else
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
#endif
        mWindow = glfwCreateWindow(mWidth, mHeight, mName.c_str(), nullptr,nullptr);

        if (mWindow == nullptr)
        {

            log->error("FAILED @ Make Window");
            glfwTerminate();
            return false;
        }

        // Resize callback
        glfwSetFramebufferSizeCallback(mWindow, FramebufferSizeCallback);
        glfwSwapInterval(1);
        return true;
    }

    bool
    DTWindowComponent::initImGui
    ()
    {
        auto log = getLog();
        log->debug("Initialising ImGui");
        char* glsl_version = "#version 330";
        // Setup Dear ImGui binding
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

        ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        // Setup style
        //ImGui::StyleColorsDark();

        return true;
    }

    bool
    DTWindowComponent::initGL
    ()
    {
        auto log = spdlog::get("GLFWWindowComponent");
        log->debug("Initialising GLFW::OpenGL");
        /* Make the window's context current */
        glfwMakeContextCurrent(mWindow);
        return true;
    }

    void
    DTWindowComponent::updateComponent
    ()
    {
        auto log = spdlog::get("GLFWWindowComponent");
        auto asr = mActiveSceneRuntime;
        if (asr == nullptr)
        {
            log->error("No active scene runtime");
            return;
        }

        glfwPollEvents();

        if(glfwWindowShouldClose(mWindow))
        {
            mActiveSceneRuntime->setState(Dream::SCENE_STATE_STOPPED);
        }

        if (WindowSizeChanged)
        {
            getCurrentDimensions();
            WindowSizeChanged = false;
        }

    }

    void DTWindowComponent::getCurrentDimensions()
    {
        glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);
        cout << "Framebuffer Size Changed: " << mWidth << "," << mHeight << endl;
        mSizeHasChanged = true;
    }

    void DTWindowComponent::swapBuffers()
    {
        if (mWindow != nullptr)
        {
            glfwSwapBuffers(mWindow);
        }
    }

    void
    DTWindowComponent::drawImGui
    ()
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Rendering
        for (DTWidget* widget : mWidgets)
        {
            widget->draw();
        }

        // End Rendering
        ImGui::Render();
        glfwMakeContextCurrent(mWindow);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwMakeContextCurrent(mWindow);
    }

    void
    DTWindowComponent::cleanUpImGui
    ()
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void
    DTWindowComponent::addWidget
    (DTWidget* widget)
    {
        auto log = getLog();
        auto end = mWidgets.end();
        auto itr = find(mWidgets.begin(), end, widget);

        if (itr == end)
        {
            log->debug("Adding Widget {}", widget->getClassName());
            mWidgets.push_back(widget);
        }
        else
        {
            log->error("Widget {} is all ready registered",widget->getClassName());
        }
    }

    void
    DTWindowComponent::removeWidget
    (DTWidget* widget)
    {
        auto log = getLog();
        auto end = mWidgets.end();
        auto itr = find(mWidgets.begin(), end, widget);
        if (itr != end)
        {
            log->debug("Removig instance of {} from widgets", widget->getClassName());
            mWidgets.erase(itr);
        }
        else
        {
            log->error("This instane of {} was not in the widgets list",widget->getClassName());
        }
    }
} // End of Dream


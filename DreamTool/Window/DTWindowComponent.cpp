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


#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include "DTWindowComponent.h"
#include "../Widgets/ImGui/ImGuiWidget.h"
#include "../Widgets/GL/GLWidget.h"
#include "../deps/ImGui/imgui_internal.h"
#include "../deps/ImGui/ImGuizmo.h"
#include "../../DreamCore/Scene/SceneRuntime.h"

using namespace Dream;


static bool WindowSizeChanged = false;

namespace DreamTool
{
    void
    FramebufferSizeCallback
    (GLFWwindow*, int, int)
    {
        WindowSizeChanged = true;
    }

    DTWindowComponent::DTWindowComponent
    () : WindowComponent(),
         mDPIScaleX(1.0f),
         mDPIScaleY(1.0f)
    {
        setLogClassName("DTWindowComponent");
        auto log = getLog();
        log->info("Constructing" );
        mName = "Dream";
    }

    DTWindowComponent::~DTWindowComponent
    ()
    {
        auto log = getLog();
        log->info("Destructing" );
        cleanUpImGui();
        glfwTerminate();
        mWindow = nullptr;
    }

    GLFWwindow*DTWindowComponent::getGlfwWindow()
    {
       return mWindow;
    }

    bool
    DTWindowComponent::init
    ()
    {
        if (!initGLFW())
        {
            return false;
        }

        if (!initGL())
        {
            return false;
        }

        if (!initImGui())
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
        checkGLError();
    }

    bool
    DTWindowComponent::initGLFW
    ()
    {
        auto log = getLog();
        log->debug("Initialising GLFW");

        /* Initialize the library */
        if (!glfwInit())
        {
            log->error("FAILED @ Initialising GLFW");
            return false;
        }

        /* Create a windowed mode window and its OpenGL context */
#ifdef WIN32
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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
        glfwSwapInterval(0);
        //glfwGetMonitorContentScale(glfwGetPrimaryMonitor(),mDPIScaleX,mDPIScaleY); Requires GLFW >=3.3
        glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);
        log->debug("Queried Framebuffer size as {}x{}",mWidth,mHeight);
        return true;
    }

    bool
    DTWindowComponent::initImGui
    ()
    {
        auto log = getLog();
        log->debug("Initialising ImGui");
        const char* glsl_version = "#version 330";
        // Setup Dear ImGui binding
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        setTheme();
        setFont();
        return true;
    }

    bool
    DTWindowComponent::initGL
    ()
    {
        auto log = getLog();
        log->debug("Initialising GLFW::OpenGL");
        glfwMakeContextCurrent(mWindow);

        glewExperimental = GL_TRUE;
        GLenum glewInitResult = glewInit();

        if (glewInitResult != GLEW_OK)
        {
            log->error("GLEW failed to initialise");
            return false;
        }

        checkGLError();

        log->debug(
            "OpenGL Version {}, Shader Version {}",
            glGetString(GL_VERSION),
            glGetString(GL_SHADING_LANGUAGE_VERSION)
        );
        return true;
    }

    void
    DTWindowComponent::updateComponent
    (SceneRuntime* sr)
    {
        auto log = getLog();

        glfwPollEvents();

        if(glfwWindowShouldClose(mWindow))
        {
            if (sr != nullptr)
            {
                sr->setState(Dream::SCENE_STATE_TO_DESTROY);
            }
            setShouldClose(true);
            log->error("Window should close");
        }

        if (WindowSizeChanged)
        {
            getCurrentDimensions();
            WindowSizeChanged = false;
        }
    }

    void DTWindowComponent::getCurrentDimensions()
    {
        auto log = getLog();
        glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);
        log->error("Framebuffer Size Changed: {}x{}", mWidth ,mHeight);
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
    DTWindowComponent::drawGLWidgets
    ()
    {
        for (GLWidget* widget : mGLWidgets)
        {
            if(widget->getVisible())
            {
                widget->draw();
            }
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
        ImGuizmo::BeginFrame();
        ImGuizmo::Enable(true);
        // Rendering
        for (ImGuiWidget* widget : mImGuiWidgets)
        {
            if (widget->getVisible())
            {
                widget->draw();
            }
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

    void DTWindowComponent::setTheme()
    {
    }

    void DTWindowComponent::setFont()
    {

        ImGuiIO& io = ImGui::GetIO();
        RegularFont = io.Fonts->AddFontFromFileTTF("./fonts/Roboto-Medium.ttf", mUiFontSize);
        MonoFont = io.Fonts->AddFontFromFileTTF("./fonts/SourceCodePro-Regular.ttf", mMonoFontSize);
    }

    float DTWindowComponent::getMonoFontSize() const
    {
        return mMonoFontSize;
    }

    void DTWindowComponent::setMonoFontSize(float monoFontSize)
    {
        mMonoFontSize = monoFontSize;
    }

    float DTWindowComponent::getUiFontSize() const
    {
        return mUiFontSize;
    }

    void DTWindowComponent::setUiFontSize(float uiFontSize)
    {
        mUiFontSize = uiFontSize;
    }

    void
    DTWindowComponent::addImGuiWidget
    (ImGuiWidget* widget)
    {
        auto log = getLog();
        auto end = mImGuiWidgets.end();
        auto itr = find(mImGuiWidgets.begin(), end, widget);

        if (itr == end)
        {
            log->debug("Adding Widget {}", widget->getClassName());
            mImGuiWidgets.push_back(widget);
        }
        else
        {
            log->error("Widget {} is all ready registered",widget->getClassName());
        }
    }

    void
    DTWindowComponent::removeImGuiWidget
    (ImGuiWidget* widget)
    {
        auto log = getLog();
        auto end = mImGuiWidgets.end();
        auto itr = find(mImGuiWidgets.begin(), end, widget);
        if (itr != end)
        {
            log->debug("Removig instance of {} from widgets", widget->getClassName());
            mImGuiWidgets.erase(itr);
        }
        else
        {
            log->error("This instane of {} was not in the widgets list",widget->getClassName());
        }
    }

    void
    DTWindowComponent::addGLWidget
    (GLWidget* widget)
    {
        auto log = getLog();
        auto end = mGLWidgets.end();
        auto itr = find(mGLWidgets.begin(), end, widget);

        if (itr == end)
        {
            log->debug("Adding Widget {}", widget->getClassName());
            mGLWidgets.push_back(widget);
        }
        else
        {
            log->error("Widget {} is all ready registered",widget->getClassName());
        }
    }

    void
    DTWindowComponent::removeGLWidget
    (GLWidget* widget)
    {
        auto log = getLog();
        auto end = mGLWidgets.end();
        auto itr = find(mGLWidgets.begin(), end, widget);
        if (itr != end)
        {
            log->debug("Removig instance of {} from widgets", widget->getClassName());
            mGLWidgets.erase(itr);
        }
        else
        {
            log->error("This instane of {} was not in the widgets list",widget->getClassName());
        }
    }

    ImFont* DTWindowComponent::RegularFont = nullptr;
    ImFont* DTWindowComponent::MonoFont = nullptr;
} // End of Dream


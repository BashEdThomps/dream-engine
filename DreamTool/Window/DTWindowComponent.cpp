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


#include <DreamCore.h>
#include "DTWindowComponent.h"
#include "Widgets/ImGui/ImGuiWidget.h"
#include "Widgets/GL/GLWidget.h"
#include <imgui_internal.h>
#include <ImGuizmo.h>

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
    void
    GLFWErrorCallback
    (int _errno, const char* errmsg)
    {
        cout << "GLFW Error: Number " << _errno << "\nMessage: " << errmsg << endl;
    }

    DTWindowComponent::DTWindowComponent
    () : WindowComponent(),
         mWindow(nullptr),
         mUiFontSize(16.0f),
         mDPIScaleX(1.0f),
         mDPIScaleY(1.0f)
    {
        LOG_INFO("Constructing" );
        mName = "Dream";
    }

    DTWindowComponent::~DTWindowComponent
    ()
    {
        LOG_INFO("Destructing" );
        cleanUpImGui();
        if (mWindow)
        {
            glfwTerminate();
            mWindow = nullptr;
        }
    }

    GLFWwindow*DTWindowComponent::getGlfwWindow()
    {
       return mWindow;
    }

    bool
    DTWindowComponent::init
    ()
    {
        LOG_TRACE("{}", __FUNCTION__ );
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
        GLCheckError();
    }

    bool
    DTWindowComponent::initGLFW
    ()
    {
        LOG_DEBUG("Initialising GLFW");
        glfwSetErrorCallback(GLFWErrorCallback);

        /* Initialize the library */
        if (!glfwInit())
        {
            LOG_ERROR("FAILED @ Initialising GLFW");
            return false;
        }

        /* Create a windowed mode window and its OpenGL context */
        //glfwWindowHint(GLFW_SAMPLES, 8);
        #ifdef WIN32
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif
        #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
        #endif
        #ifdef __linux__
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
        #endif
        mWindow = glfwCreateWindow(mWidth, mHeight, mName.c_str(), nullptr,nullptr);

        if (mWindow == nullptr)
        {

            LOG_ERROR("FAILED @ Make Window");
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(mWindow);

        // Resize callback
        glfwSetFramebufferSizeCallback(mWindow, FramebufferSizeCallback);
        glfwSwapInterval(1);
        //glfwGetMonitorContentScale(glfwGetPrimaryMonitor(),mDPIScaleX,mDPIScaleY); Requires GLFW >=3.3
        glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);

        LOG_DEBUG("Queried Framebuffer size as {}x{}",mWidth,mHeight);
        return true;
    }

    bool
    DTWindowComponent::initImGui
    ()
    {

#ifdef IMGUI_HAS_DOCK
        const char* glsl_version = "#version 330 core";
		// Setup Dear ImGui binding
		IMGUI_CHECKVERSION();
		auto ctx = ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Dockable Windows
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

		// When viewports are enabled we tweak WindowRounding/WindowBg so
		// platform windows can look identical to regular ones.

	   ImGuiStyle& style = ImGui::GetStyle();
	   if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	   {
		   style.WindowRounding = 0.0f;
		   style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	   }

		ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
		GLCheckError();
		ImGui_ImplOpenGL3_Init(glsl_version);
		GLCheckError();
		setTheme();
		setFont();
		return true;
#else
        LOG_DEBUG("Initialising ImGui");
        const char* glsl_version = "#version 330 core";
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
#endif
    }

    bool
    DTWindowComponent::initGL
    ()
    {
        LOG_DEBUG("Initialising GLFW::OpenGL");

        if(!gladLoadGL())
		{
			LOG_ERROR("Window: Error initialising GLAD!\n");
			return false;
		}
		//glViewport(0,0,mWidth,mHeight);
        GLCheckError();

        LOG_DEBUG(
            "OpenGL Version {}, Shader Version {}",
            glGetString(GL_VERSION),
            glGetString(GL_SHADING_LANGUAGE_VERSION)
        );
        return true;
    }

    void
    DTWindowComponent::updateWindow
    (SceneRuntime* sr)
    {
        glfwPollEvents();

        if(glfwWindowShouldClose(mWindow))
        {
            if (sr != nullptr)
            {
                sr->setState(Dream::SCENE_STATE_TO_DESTROY);
            }
            setShouldClose(true);
            LOG_ERROR("Window should close");
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
        LOG_ERROR("Framebuffer Size Changed: {}x{}", mWidth ,mHeight);
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
#ifdef IMGUI_HAS_DOCK
		//debug("Window: {}",__FUNCTION__);
		ImGuiIO& io = ImGui::GetIO();
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// Rendering

		//ImGui::PushFont(mDefaultFont);

		static bool p_open = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		//if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		//	window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", &p_open, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		//debug("Window {} ImGui Widgets", mImGuiWidgets.size());

        ImGuizmo::BeginFrame();
        ImGuizmo::Enable(true);

        for (ImGuiWidget* widget : mImGuiWidgets)
		{
			if (widget->getVisible())
			{
				widget->draw();
			}
		}

		//ImGui::PopFont();
		ImGui::End();

		// End Rendering
		ImGui::Render();
		glfwMakeContextCurrent(mWindow);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwMakeContextCurrent(mWindow);

		// Update and Render additional Platform Windows
		 // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		 //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		 if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		 {
			 GLFWwindow* backup_current_context = glfwGetCurrentContext();
			 ImGui::UpdatePlatformWindows();
			 ImGui::RenderPlatformWindowsDefault();
			 glfwMakeContextCurrent(backup_current_context);
		 }
		GLCheckError();
#else
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
#endif
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
        auto end = mImGuiWidgets.end();
        auto itr = find(mImGuiWidgets.begin(), end, widget);

        if (itr == end)
        {
            LOG_DEBUG("Adding Widget");
            mImGuiWidgets.push_back(widget);
        }
        else
        {
            LOG_ERROR("Widget is all ready registered");
        }
    }

    void
    DTWindowComponent::removeImGuiWidget
    (ImGuiWidget* widget)
    {
        auto end = mImGuiWidgets.end();
        auto itr = find(mImGuiWidgets.begin(), end, widget);
        if (itr != end)
        {
            LOG_DEBUG("Removig Runtime of widget from widgets");
            mImGuiWidgets.erase(itr);
        }
        else
        {
            LOG_ERROR("This instane of widget was not in the widgets list");
        }
    }

    void
    DTWindowComponent::addGLWidget
    (GLWidget* widget)
    {
        auto end = mGLWidgets.end();
        auto itr = find(mGLWidgets.begin(), end, widget);

        if (itr == end)
        {
            LOG_DEBUG("Adding Widget");
            mGLWidgets.push_back(widget);
        }
        else
        {
            LOG_ERROR("Widget is all ready registered");
        }
    }

    void
    DTWindowComponent::removeGLWidget
    (GLWidget* widget)
    {
        auto end = mGLWidgets.end();
        auto itr = find(mGLWidgets.begin(), end, widget);
        if (itr != end)
        {
            LOG_DEBUG("Removig Runtime of from widgets");
            mGLWidgets.erase(itr);
        }
        else
        {
            LOG_ERROR("This instane of {} was not in the widgets list");
        }
    }

    ImFont* DTWindowComponent::RegularFont = nullptr;
    ImFont* DTWindowComponent::MonoFont = nullptr;
} // End of Dream


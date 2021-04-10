#include <DreamCore.h>
#include "DreamToolWindow.h"
#include "Widgets/ImGui/ImGuiWidget.h"
#include "Widgets/GL/GLWidget.h"
#include <imgui_internal.h>

static bool WindowSizeChanged = false;

namespace octronic::dream::tool
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
    LOG_ERROR("GLFW Error: Number {}\nMessage: {}",_errno, errmsg);
  }

  void
  MouseWheelCallback
  (GLFWwindow* , double xoff, double yoff)
  {
    DreamToolWindow::MouseWheelX = xoff;
    DreamToolWindow::MouseWheelY = yoff;
  }

  DreamToolWindow::DreamToolWindow()
    : mWindow(nullptr),
      mUiFontSize(16.0f),
      mDPIScaleX(1.0f),
      mDPIScaleY(1.0f),
      mWidth(1024),
      mHeight(768),
      mShouldClose(false),
      mSizeHasChanged(false)
  {
    LOG_INFO("DreamToolWindow: Constructing" );
  }

  DreamToolWindow::~DreamToolWindow
  ()
  {
    LOG_INFO("DreamToolWindow: Destructing" );
    cleanUpImGui();
    if (mWindow)
    {
      glfwTerminate();
      mWindow = nullptr;
    }
  }

  GLFWwindow*DreamToolWindow::getGlfwWindow()
  {
    return mWindow;
  }

  bool
  DreamToolWindow::init
  ()
  {
    LOG_TRACE("DreamToolWindow: {}", __FUNCTION__ );
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
  DreamToolWindow::bindDefaultFrameBuffer
  ()
  {
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    GLCheckError();
  }

  bool
  DreamToolWindow::initGLFW
  ()
  {
    LOG_DEBUG("DreamToolWindow: Initialising GLFW");
    glfwSetErrorCallback(GLFWErrorCallback);

    /* Initialize the library */
    if (!glfwInit())
    {
      LOG_ERROR("DreamToolWindow: FAILED @ Initialising GLFW");
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
    glfwWindowHint(GLFW_SAMPLES, 4);
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
    mWindow = glfwCreateWindow(mWidth, mHeight, "DreamTool", nullptr,nullptr);

    if (mWindow == nullptr)
    {

      LOG_ERROR("DreamToolWindow: FAILED @ Make Window");
      glfwTerminate();
      return false;
    }

    glfwMakeContextCurrent(mWindow);

    // Resize callback
    glfwSetFramebufferSizeCallback(mWindow, FramebufferSizeCallback);
    glfwSetScrollCallback(mWindow, MouseWheelCallback);
    glfwSwapInterval(1);
    //glfwGetMonitorContentScale(glfwGetPrimaryMonitor(),mDPIScaleX,mDPIScaleY); Requires GLFW >=3.3
    glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);

    LOG_DEBUG("DreamToolWindow: Queried Framebuffer size as {}x{}",mWidth,mHeight);
    return true;
  }

  bool
  DreamToolWindow::initImGui
  ()
  {
    const char* glsl_version = "#version 330 core";
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
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
  }

  bool
  DreamToolWindow::initGL
  ()
  {
    LOG_DEBUG("DreamToolWindow: Initialising GLFW::OpenGL");

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      LOG_ERROR("DreamToolWindow: Window: Error initialising GLAD!\n");
      return false;
    }
    //glViewport(0,0,mWidth,mHeight);
    GLCheckError();

    LOG_DEBUG(
          "DreamToolWindow: OpenGL Version {}, Shader Version {}",
          glGetString(GL_VERSION),
          glGetString(GL_SHADING_LANGUAGE_VERSION)
          );
    return true;
  }

  void
  DreamToolWindow::update
  ()
  {
    glfwPollEvents();

    if(glfwWindowShouldClose(mWindow))
    {
      mShouldClose = true;
      LOG_ERROR("DreamToolWindow: Window should close");
    }

    if (WindowSizeChanged)
    {
      getCurrentDimensions();
      WindowSizeChanged = false;
    }
  }

  void DreamToolWindow::getCurrentDimensions()
  {
    glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);
    LOG_ERROR("DreamToolWindow: Framebuffer Size Changed: {}x{}", mWidth ,mHeight);
    mSizeHasChanged = true;
  }

  void DreamToolWindow::swapBuffers()
  {
    if (mWindow != nullptr)
    {
      glfwSwapBuffers(mWindow);
    }
  }

  void
  DreamToolWindow::drawGLWidgets
  ()
  {
    for (GLWidget& widget : mGLWidgets)
    {
      if(widget.getVisible())
      {
        widget.draw();
      }
    }
  }


  void
  DreamToolWindow::drawImGui
  ()
  {
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
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
      window_flags |= ImGuiWindowFlags_NoBackground;

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

    LOG_DEBUG("DreamToolWindow: {} ImGui Widgets", mImGuiWidgets.size());


    for (ImGuiWidget& widget : mImGuiWidgets)
    {
      if (widget.getVisible())
        widget.draw();
    }

    //ImGui::PopFont();
    ImGui::End();

    // End Rendering
    ImGui::Render();
    glfwMakeContextCurrent(mWindow);
    GLCheckError();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    GLCheckError();
    glfwMakeContextCurrent(mWindow);
    GLCheckError();

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      GLFWwindow* backup_current_context = glfwGetCurrentContext();
      GLCheckError();
      ImGui::UpdatePlatformWindows();
      GLCheckError();
      ImGui::RenderPlatformWindowsDefault();
      GLCheckError();
      glfwMakeContextCurrent(backup_current_context);
      GLCheckError();
    }
    GLCheckError();
  }

  void
  DreamToolWindow::cleanUpImGui
  ()
  {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  void DreamToolWindow::setTheme()
  {
  }

  void DreamToolWindow::setFont()
  {

    ImGuiIO& io = ImGui::GetIO();
    RegularFont = io.Fonts->AddFontFromFileTTF("./fonts/Roboto-Medium.ttf", mUiFontSize);
    MonoFont = io.Fonts->AddFontFromFileTTF("./fonts/SourceCodePro-Regular.ttf", mMonoFontSize);
  }

  float DreamToolWindow::getMonoFontSize() const
  {
    return mMonoFontSize;
  }

  void DreamToolWindow::setMonoFontSize(float monoFontSize)
  {
    mMonoFontSize = monoFontSize;
  }

  float DreamToolWindow::getUiFontSize() const
  {
    return mUiFontSize;
  }

  void DreamToolWindow::setUiFontSize(float uiFontSize)
  {
    mUiFontSize = uiFontSize;
  }

  void
  DreamToolWindow::addImGuiWidget
  (ImGuiWidget& widget)
  {
    auto itr = find_if(mImGuiWidgets.begin(),
                       mImGuiWidgets.end(),
    [&](reference_wrapper<ImGuiWidget>& next)
    { return &next.get() == &widget; } );

    if (itr == mImGuiWidgets.end())
    {
      LOG_DEBUG("DreamToolWindow: Adding Widget");
      mImGuiWidgets.push_back(widget);
    }
    else
    {
      LOG_ERROR("DreamToolWindow: Widget is all ready registered");
    }
  }

  void
  DreamToolWindow::removeImGuiWidget
  (ImGuiWidget& widget)
  {
    auto itr = find_if(mImGuiWidgets.begin(), mImGuiWidgets.end(),
                    [&](reference_wrapper<ImGuiWidget> next)
    { return &next.get() == &widget; });

    if (itr != mImGuiWidgets.end())
    {
      LOG_DEBUG("DreamToolWindow: Removig Runtime of widget from widgets");
      mImGuiWidgets.erase(itr);
    }
    else
    {
      LOG_ERROR("DreamToolWindow: This instane of widget was not in the widgets list");
    }
  }

  void
  DreamToolWindow::addGLWidget
  (GLWidget& widget)
  {
    auto itr = find_if(mGLWidgets.begin(),
                    mGLWidgets.end(),
    [&](reference_wrapper<GLWidget>& next)
    {return &next.get() == &widget; });

    if (itr == mGLWidgets.end())
    {
      LOG_DEBUG("DreamToolWindow: Adding Widget");
      mGLWidgets.push_back(widget);
    }
    else
    {
      LOG_ERROR("DreamToolWindow: Widget is all ready registered");
    }
  }

  void
  DreamToolWindow::removeGLWidget
  (GLWidget& widget)
  {
    auto itr = find_if(mGLWidgets.begin(), mGLWidgets.end(),
                    [&](reference_wrapper<GLWidget>& next)
    {return &next.get() == &widget;});

    if (itr != mGLWidgets.end())
    {
      LOG_DEBUG("DreamToolWindow: Removig Runtime of from widgets");
      mGLWidgets.erase(itr);
    }
    else
    {
      LOG_ERROR("DreamToolWindow: This instane of {} was not in the widgets list");
    }
  }

  bool DreamToolWindow::shouldClose() const
  {
    return mShouldClose;
  }

  ImFont* DreamToolWindow::RegularFont = nullptr;
  ImFont* DreamToolWindow::MonoFont = nullptr;
  float DreamToolWindow::MouseWheelX = 0.f;
  float DreamToolWindow::MouseWheelY = 0.f;
} // End of Dream


#include "DreamToolContext.h"
#include <nfd.h>

using octronic::dream::GraphicsComponent;
using octronic::dream::File;
using std::make_shared;

namespace octronic::dream::tool
{
  DreamToolContext::DreamToolContext(int _argc, char** _argv)
    :
      // ImGui
      mPropertiesWindow(*this),
      mProjectBrowser(*this),
      mAssetBrowser(*this),
      mSceneStateWindow(*this),
      mToolPropertiesWindow(*this),
      mMenuBar(*this),
      mRenderPipelineWindow(*this),
      mGamepadStateWindow(*this),
      mCacheContentWindow(*this),
      mTaskManagerWindow(*this),
      mScriptDebugWindow(*this),
      mGlPreviewWindowComponent(*this),
      // GL
      mGrid(*this),
      mLightViewer(*this),
      mSelectionHighlighter(*this),
      mCursor(*this),
      mPathViewer(*this),
      mPhysicsDebugDrawer(*this),
      // Models
      mTemplatesModel(*this),
      mPreferencesModel(*this),
      // Args
      mCommandArgumentsCount(_argc),
      mCommandArgumentsVector(_argv),
      mMainLoopDone(false),
      mInputTarget(INPUT_TARGET_EDITOR)
  {
    LOG_TRACE("DTContext: Constructing");
    NFD_Init();
  }

  DreamToolContext::~DreamToolContext()
  {
    LOG_TRACE("DTContext: Destructing");
    NFD_Quit();
  }

  bool DreamToolContext::init()
  {
    mWindow.setUiFontSize(16.0f);
    mWindow.setMonoFontSize(18.0f);

    if (!mWindow.init())
    {
      return false;
    }

    ScriptComponent::AddPrintListener(mScriptDebugWindow);
    FontRuntime::InitFreetypeLibrary();
    mPreferencesModel.load();
    mAudioComponent.init();

    // ImGui Widgets
    mWindow.addImGuiWidget(mPropertiesWindow);
    mWindow.addImGuiWidget(mProjectBrowser);
    mWindow.addImGuiWidget(mAssetBrowser);
    mWindow.addImGuiWidget(mScriptDebugWindow);
    mWindow.addImGuiWidget(mSceneStateWindow);
    mWindow.addImGuiWidget(mMenuBar);
    mWindow.addImGuiWidget(mToolPropertiesWindow);
    mWindow.addImGuiWidget(mRenderPipelineWindow);
    mWindow.addImGuiWidget(mGamepadStateWindow);
    mWindow.addImGuiWidget(mCacheContentWindow);
    mWindow.addImGuiWidget(mTaskManagerWindow);
    mGlPreviewWindowComponent.init();
    mWindow.addImGuiWidget(mGlPreviewWindowComponent);

    // GL Widgets

    mGrid.init();
    mGrid.setVisible(true);
    mWindow.addGLWidget(mGrid);

    mCursor.init();
    mCursor.setVisible(true);
    mWindow.addGLWidget(mCursor);

    mSelectionHighlighter.init();
    mWindow.addGLWidget(mSelectionHighlighter);

    mPhysicsDebugDrawer.init();
    mPhysicsDebugDrawer.setVisible(true);
    mWindow.addGLWidget(mPhysicsDebugDrawer);

    mLightViewer.init();
    mLightViewer.setVisible(true);
    mWindow.addGLWidget(mLightViewer);

    /*
        mPathViewer.init();
        mWindow.addGLWidget(&mPathViewer);
        */
    return true;
  }

  void DreamToolContext::run()
  {
    if (mCommandArgumentsCount > 1)
    {
      openProject(mCommandArgumentsVector[1]);
    }

    // Run the project
    while (!mMainLoopDone)
    {
      if (mWindow.shouldClose())
      {
        mMainLoopDone = true;
      }

      if (mProjectContext)
      {
        auto& pCtx = mProjectContext.value();
        auto& projectRuntimeOpt = pCtx.getProjectRuntime();
        if (projectRuntimeOpt)
        {
          auto& projectRuntime = projectRuntimeOpt.value();
          mGlPreviewWindowComponent.bindFrameBuffer();
          projectRuntime.step();
          mWindow.drawGLWidgets();
          mGlPreviewWindowComponent.updateWindow();
        }
      }


      // Draw ImGui
      mWindow.bindDefaultFrameBuffer();
      glClearColor(0.0f,0.0f,0.0f,0.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      mWindow.update();
      mWindow.drawImGui();

      if (ImGui::IsKeyPressed(GLFW_KEY_TAB,false))
      {
        switch (mInputTarget)
        {
          case INPUT_TARGET_EDITOR:
            mInputTarget = INPUT_TARGET_SCENE;
            break;
          case INPUT_TARGET_SCENE:
            mInputTarget = INPUT_TARGET_EDITOR;
            break;
        }
      }

      switch (mInputTarget)
      {
        case InputTarget::INPUT_TARGET_EDITOR:
          handleEditorInput();
          break;
        case InputTarget::INPUT_TARGET_SCENE:
          handleSceneInput();
          break;
      }
      mWindow.swapBuffers();
    }
    FontRuntime::CleanupFreetypeLibrary();
  }

  void
  DreamToolContext::handleEditorInput
  ()
  {
    // Z
    if (ImGui::IsKeyPressed(GLFW_KEY_W))
    {
      mCursor.onAction(CursorAction::ZPlus);
    }
    if (ImGui::IsKeyPressed(GLFW_KEY_S))
    {
      mCursor.onAction(CursorAction::ZMinus);
    }

    // X
    if (ImGui::IsKeyPressed(GLFW_KEY_A))
    {
      mCursor.onAction(CursorAction::XMinus);
    }
    if (ImGui::IsKeyPressed(GLFW_KEY_D))
    {
      mCursor.onAction(CursorAction::XPlus);
    }

    // Y
    if (ImGui::IsKeyPressed(GLFW_KEY_Q))
    {
      mCursor.onAction(CursorAction::YMinus);
    }
    if (ImGui::IsKeyPressed(GLFW_KEY_E))
    {
      mCursor.onAction(CursorAction::YPlus);
    }
  }

  void
  DreamToolContext::handleSceneInput
  ()
  {
    ImGuiIO& io = ImGui::GetIO();
    if (mProjectContext)
    {
      auto& pCtx = mProjectContext.value();
      auto& pRuntOpt = pCtx.getProjectRuntime();

      if (pRuntOpt)
      {
        auto& pRunt = pRuntOpt.value();
        auto& inputComp = pRunt.getInputComponent();
        auto ms = inputComp.getMouseState();
        auto ks = inputComp.getKeyboardState();
        auto js = inputComp.getJoystickState();

        // Mouse
        ms.setButtonsPressed(&io.MouseDown[0], 5);
        ms.setPosX(io.MousePos.x);
        ms.setPosY(io.MousePos.y);
        ms.setScrollX(DreamToolWindow::MouseWheelX);
        ms.setScrollY(DreamToolWindow::MouseWheelY);
        DreamToolWindow::MouseWheelX = 0.f;
        DreamToolWindow::MouseWheelY = 0.f;

        if (io.MouseWheel != 0.f || io.MouseWheelH != 0.f)
        {
          LOG_ERROR("DTContext: Got some scroll X:{} Y:{} ",
                    io.MouseWheelH, io.MouseWheel);
        }

        // Keys
        ks.setKeysPressed(&io.KeysDown[0], 512);

        // Joystick
        int jsCount = 0;
        for (int id=GLFW_JOYSTICK_1; id < GLFW_JOYSTICK_LAST; id++)
        {
          if (glfwJoystickPresent(id))
          {
            jsCount++;
            js.setName(glfwGetJoystickName(id));
            int numAxis, numButtons;
            const float* axisData = glfwGetJoystickAxes(id,&numAxis);
            const unsigned char* buttonData = glfwGetJoystickButtons(id, &numButtons);
            if (axisData != nullptr)
            {
              js.setAxisCount(numAxis);
              memcpy((void*)js.getAxisDataPointer(),axisData,sizeof(float)*numAxis);
            }
            else
            {
              js.setAxisCount(0);
            }
            if (buttonData != nullptr)
            {
              js.setButtonCount(numButtons);
              memcpy((void*)js.getButtonDataPointer(),buttonData,sizeof(unsigned char)*numButtons);
            }
            else
            {
              js.setButtonCount(0);
            }
          }

          inputComp.setKeyboardState(ks);
          inputComp.setMouseState(ms);
          inputComp.setJoystickState(js);
          inputComp.setJoystickCount(jsCount);
        }
      }
    }
  }

  void
  DreamToolContext::closeProject
  ()
  {
    mPropertiesWindow.clearPropertyTargets();
    mProjectContext.reset();
  }

  bool
  DreamToolContext::openProject
  (const string& dir)
  {
    closeProject();

    LOG_DEBUG("DTContext: Opening project {}",dir);
    mLastDirectory = dir;
    mProjectContext.emplace(getGlPreviewWindowComponent(), getAudioComponent(), getStorageManager(), dir);

    auto& pCtx = mProjectContext.value();

    if(pCtx.openFromPath())
    {
      auto& pDefOpt = pCtx.getProjectDefinition();
      if (pDefOpt)
      {
        auto& pDef = pDefOpt.value();
        stringstream ss;
        ss << "Opened Project: ";
        ss << pDef.getName();
        mMenuBar.setMessageString(ss.str());
        return true;
      }
    }
    return false;
  }

  bool
  DreamToolContext::newProject
  (const string& dir)
  {
    LOG_DEBUG("DTContext: Creating Project {}",dir);

    mLastDirectory = dir;
    closeProject();
    mProjectContext.emplace(getGlPreviewWindowComponent(), getAudioComponent(), getStorageManager(), dir);

    auto& pCtx = mProjectContext.value();

    if(pCtx.newProject(dir))
    {
      stringstream ss;
      ss  << "Created Project in: ";
      ss << dir;
      mMenuBar.setMessageString(ss.str());
      return true;
    }
    else
    {
      mMenuBar.setMessageString("Project already exists in this directory!");
    }
    return false;
  }

  // ImGui Widgets
  DreamToolWindow&
  DreamToolContext::getWindow
  ()
  {
    return mWindow;
  }

  optional<ProjectContext>&
  DreamToolContext::getProjectContext
  ()
  {
    return mProjectContext;
  }

  PropertiesWindow&
  DreamToolContext::getPropertiesWindow
  ()
  {
    return mPropertiesWindow;
  }

  ProjectBrowser&
  DreamToolContext::getProjectBrowser
  ()
  {
    return mProjectBrowser;
  }

  AssetBrowser&
  DreamToolContext::getAssetBrowser()
  {
    return mAssetBrowser;
  }

  ScriptDebugWindow&
  DreamToolContext::getScriptDebugWindow
  ()
  {
    return mScriptDebugWindow;
  }

  SceneStateWindow&
  DreamToolContext::getSceneStateWindow
  ()
  {
    return mSceneStateWindow;
  }

  ToolPropertiesWindow&
  DreamToolContext::getToolPropertiesWindow
  ()
  {
    return mToolPropertiesWindow;
  }

  MenuBar&
  DreamToolContext::getMenuBar
  ()
  {
    return mMenuBar;
  }

  RenderingDebugWindow&
  DreamToolContext::getRenderPipelineWindow
  ()
  {
    return mRenderPipelineWindow;
  }

  GamepadStateWindow&
  DreamToolContext::getGamepadStateWindow
  ()
  {
    return mGamepadStateWindow;
  }

  CacheContentWindow&
  DreamToolContext::getCacheContentWindow
  ()
  {
    return mCacheContentWindow;
  }

  TaskManagerWindow&
  DreamToolContext::getTaskManagerWindow
  ()
  {
    return mTaskManagerWindow;
  }

  GLPreviewWindowComponent&
  DreamToolContext::getGlPreviewWindowComponent
  ()
  {
    return mGlPreviewWindowComponent;
  }

  Grid&
  DreamToolContext::getGrid
  ()
  {
    return mGrid;
  }

  LightViewer&
  DreamToolContext::getLightViewer
  ()
  {
    return mLightViewer;
  }

  SelectionHighlighter&
  DreamToolContext::getSelectionHighlighter
  ()
  {
    return mSelectionHighlighter;
  }

  Cursor&
  DreamToolContext::getCursor
  ()
  {
    return mCursor;
  }

  PathViewer&
  DreamToolContext::getPathViewer
  ()
  {
    return mPathViewer;
  }

  // Data Models
  TemplatesModel&
  DreamToolContext::getTemplatesModel
  ()
  {
    return mTemplatesModel;
  }

  PreferencesModel&
  DreamToolContext::getPreferencesModel
  ()
  {
    return mPreferencesModel;
  }

  StorageManager&
  DreamToolContext::getStorageManager
  ()
  {
    return mStorageManager;
  }

  int
  DreamToolContext::getCommandArgumentsCount()
  {
    return mCommandArgumentsCount;
  }

  char**
  DreamToolContext::getCommandArgumentsVector()
  {
    return mCommandArgumentsVector;
  }

  bool
  DreamToolContext::getMainLoopDone()
  {
    return mMainLoopDone;
  }

  void
  DreamToolContext::setMainLoopDone(bool b)
  {
    mMainLoopDone = b;
  }

  InputTarget
  DreamToolContext::getInputTarget()
  {
    return mInputTarget;
  }

  void
  DreamToolContext::setInputTarget
  (InputTarget t)
  {
    mInputTarget = t;
  }

  string
  DreamToolContext::getLastDirectory()
  {
    return mLastDirectory;
  }

  void
  DreamToolContext::setLastDirectory(const string& lastDir)
  {
    mLastDirectory = lastDir;
  }

  bool
  DreamToolContext::openInExternalEditor
  (AssetDefinition& definition, const string& format)
  {
    if (mPreferencesModel.getExternalEditorPath().empty())
    {
      LOG_ERROR("DTContext: External editor has not been set");
      return false;
    }

    if (mProjectContext)
    {
      auto& pCtx = mProjectContext.value();
      auto& pDirOpt = pCtx.getProjectDirectory();
      if (pDirOpt)
      {
        auto& pDir = pDirOpt.value();
        string filePath = pDir.getAssetAbsolutePath(definition,format);
        auto& sm = getStorageManager();
        auto& targetFile = sm.openFile(filePath);

        if (!targetFile.exists())
        {
          LOG_DEBUG("DTContext: Attempted to open file that does not exist {}", targetFile.getPath());
          return false;
        }

        sm.closeFile(targetFile);
        char command_buffer[256] = {0};
#if defined (__APPLE__)
        sprintf(command_buffer, "open \"%s\" -a \"%s\"",
                filePath.c_str(),
                mPreferencesModel.getExternalEditorPath().c_str());
#else
#endif
        LOG_DEBUG("DTContext: Opening file with command {}",command_buffer);
        int result = system((char *)command_buffer);
        return result == 0;
      }
    }
    return false;
  }

  OpenALAudioComponent&
  DreamToolContext::getAudioComponent
  ()
  {
    return mAudioComponent;
  }
}

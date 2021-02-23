#include "DreamToolContext.h"
#include <nfd.h>

using octronic::dream::GraphicsComponent;
using octronic::dream::Project;
using octronic::dream::File;

namespace octronic::dream::tool
{
    DreamToolContext::DreamToolContext(int _argc, char** _argv)
        :mProject(nullptr),
          // ImGui
          mPropertiesWindow(this),
          mProjectBrowser(this),
          mScriptDebugWindow(this),
          mSceneStateWindow(this),
          mToolPropertiesWindow(this),
          mMenuBar(this),
          mRenderPipelineWindow(this),
          mGamepadStateWindow(this),
          mCacheContentWindow(this),
          mTaskManagerWindow(this),
          mGlPreviewWindowComponent(this),
          // GL
          mGrid(this),
          mLightViewer(this),
          mSelectionHighlighter(this),
          mCursor(this),
          mPathViewer(this),
          mAnimationViewer(this),
          // Models
          mProjectDirectory(&mStorageManager),
          mModelDefinitionBatchImporter(this),
          mTemplatesModel(this),
          mPreferencesModel(this),
          // Args
          mCommandArgumentsCount(_argc),
          mCommandArgumentsVector(_argv),
          mMainLoopDone(false),
          mInputTarget(INPUT_TARGET_EDITOR),
          mProjectBaseDirectory(nullptr)
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
#ifdef WIN32
        windowComponent.setUiFontSize(24.0f);
        windowComponent.setMonoFontSize(24.0f);
#else
        mWindow.setUiFontSize(16.0f);
        mWindow.setMonoFontSize(18.0f);
#endif
        if (!mWindow.init())
        {
            return false;
        }

        ScriptComponent::AddPrintListener(&mScriptDebugWindow);
        FontRuntime::InitFreetypeLibrary();
        mPreferencesModel.load();

        // ImGui Widgets
        mWindow.addImGuiWidget(&mPropertiesWindow);
        mWindow.addImGuiWidget(&mProjectBrowser);
        mWindow.addImGuiWidget(&mScriptDebugWindow);
        mWindow.addImGuiWidget(&mSceneStateWindow);
        mWindow.addImGuiWidget(&mMenuBar);
        mWindow.addImGuiWidget(&mToolPropertiesWindow);
        mWindow.addImGuiWidget(&mRenderPipelineWindow);
        mWindow.addImGuiWidget(&mGamepadStateWindow);
        mWindow.addImGuiWidget(&mCacheContentWindow);
        mWindow.addImGuiWidget(&mTaskManagerWindow);
        mWindow.addImGuiWidget(&mGlPreviewWindowComponent);

        // GL Widgets
        mGlPreviewWindowComponent.init();
        mAudioComponent.init();
        mGrid.init();
        mLightViewer.init();
        mSelectionHighlighter.init();
        mCursor.init();
        mPathViewer.init();
        mAnimationViewer.init();
        mWindow.addGLWidget(&mGrid);
        mWindow.addGLWidget(&mLightViewer);
        mWindow.addGLWidget(&mSelectionHighlighter);
        mWindow.addGLWidget(&mCursor);
        mWindow.addGLWidget(&mPathViewer);
        mWindow.addGLWidget(&mAnimationViewer);
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

            if (mProject)
            {
                ProjectRuntime* projectRuntime = mProject->getRuntime();
                if (projectRuntime)
                {
                    projectRuntime->step();
					mGlPreviewWindowComponent.updateWindow();
                }
            }

            // Draw ImGui
            mWindow.bindDefaultFrameBuffer();
            glClearColor(0.0f,0.0f,0.0f,0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            mWindow.update();
            mWindow.drawGLWidgets();
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

            if (mProject && mProject->getRuntime())
            {
                auto sr = mProject->getRuntime()->getActiveSceneRuntime();
                if (sr)
                {
                    switch (mInputTarget)
                    {
                        case InputTarget::INPUT_TARGET_EDITOR:
                            handleEditorInput(sr);
                            break;
                        case InputTarget::INPUT_TARGET_SCENE:
                            handleSceneInput(sr);
                            break;
                    }
                }
            }
            mWindow.swapBuffers();
        }

        FontRuntime::CleanupFreetypeLibrary();
    }

    void
    DreamToolContext::handleEditorInput
    (SceneRuntime* sr)
    {
        static ImGuiIO& io = ImGui::GetIO();
        static float mouseScalar = 0.001f;
        auto camera = sr->getCamera();
        bool sendKeysToCamera = false;
        if (!io.WantCaptureMouse)
        {
#ifdef __APPLE__
            if (io.MouseDown[0])
            {
                sendKeysToCamera = true;
            }
            mCursor.setMousePosition(io.MousePos.x, io.MousePos.y);
#else
            if (io.MouseDown[2])
            {
                sendKeysToCamera = true;
            }
#endif
        }

        if (!io.WantCaptureKeyboard)
        {
            if (sendKeysToCamera)
            {
            }
            // Send to cursor
            else
            {
                // Z
                if (ImGui::IsKeyPressed(GLFW_KEY_W))
                {
                    mCursor.onAction(Cursor::ZPlus);
                }
                if (ImGui::IsKeyPressed(GLFW_KEY_S))
                {
                    mCursor.onAction(Cursor::ZMinus);
                }

                // X
                if (ImGui::IsKeyPressed(GLFW_KEY_A))
                {
                    mCursor.onAction(Cursor::XMinus);
                }
                if (ImGui::IsKeyPressed(GLFW_KEY_D))
                {
                    mCursor.onAction(Cursor::XPlus);
                }

                // Y
                if (ImGui::IsKeyPressed(GLFW_KEY_Q))
                {
                    mCursor.onAction(Cursor::YMinus);
                }
                if (ImGui::IsKeyPressed(GLFW_KEY_E))
                {
                    mCursor.onAction(Cursor::YPlus);
                }
            }
        }
    }

    void
    DreamToolContext::handleSceneInput
    (SceneRuntime* sRunt)
    {
        ImGuiIO& io = ImGui::GetIO();
        auto pRunt = mProject->getRuntime();
        if (pRunt)
        {
            auto inputComp = pRunt->getInputComponent();
            if (inputComp)
            {
                MouseState& ms = inputComp->getMouseState();
                KeyboardState& ks = inputComp->getKeyboardState();
                JoystickState& js = inputComp->getJoystickState();

                // Mouse
                memcpy(ms.ButtonsDown, &io.MouseDown[0], sizeof(bool)*5);
                ms.PosX = io.MousePos.x;
                ms.PosY = io.MousePos.y;
                ms.ScrollX = io.MouseWheelH;
                ms.ScrollY = io.MouseWheel;

                // Keys
                memcpy(ks.KeysDown, &io.KeysDown[0], sizeof(bool)*512);

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
                }
                inputComp->setJoystickCount(jsCount);
            }
        }
    }

    void
    DreamToolContext::closeProject
    ()
    {
        mPropertiesWindow.clearPropertyTargets();
        mProjectDirectory.closeProject();
        mProject = nullptr;
        if (mProjectBaseDirectory)
        {
            mStorageManager.closeDirectory(mProjectBaseDirectory);
        }
        mProjectBaseDirectory = nullptr;
    }

    bool
    DreamToolContext::openProject
    (const string& dir)
    {
        closeProject();

        LOG_DEBUG("DTContext: Opening project {}",dir);
        mLastDirectory = dir;
        mProjectBaseDirectory = mStorageManager.openDirectory(dir);
        mProject = mProjectDirectory.openFromDirectory(mProjectBaseDirectory);
        if(mProject)
        {
            mProject->setAudioComponent(&mAudioComponent);
            mProject->setWindowComponent(&mGlPreviewWindowComponent);
            mProject->createProjectRuntime();
            stringstream ss;
            ss  << "Opened Project: "
                << mProject->getDefinition()->getName();
            mMenuBar.setMessageString(ss.str());
            return true;
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
        mProjectBaseDirectory = mStorageManager.openDirectory(dir);
        mProject = mProjectDirectory.newProject(mProjectBaseDirectory);

        if(mProject)
        {
            mProject->setWindowComponent(&mGlPreviewWindowComponent);
            mProject->createProjectRuntime();
            stringstream ss;
            ss  << "Created Project in: "
                << dir;
            mMenuBar.setMessageString(ss.str());
            return true;
        }
        else
        {
            stringstream ss;
            ss  << "Project already exists in this directory!";
            mMenuBar.setMessageString(ss.str());
        }
        return false;
    }

    // ImGui Widgets
    DreamToolWindow*
    DreamToolContext::getWindow()
    {
        return &mWindow;
    }

    octronic::dream::Project*
    DreamToolContext::getProject()
    {
        return mProject;
    }

    PropertiesWindow*
    DreamToolContext::getPropertiesWindow()
    {
        return &mPropertiesWindow;
    }

    ProjectBrowser*
    DreamToolContext::getProjectBrowser()
    {
        return &mProjectBrowser;
    }

    ScriptDebugWindow*
    DreamToolContext::getScriptDebugWindow()
    {
        return &mScriptDebugWindow;
    }

    SceneStateWindow*
    DreamToolContext::getSceneStateWindow()
    {
        return &mSceneStateWindow;
    }

    ToolPropertiesWindow*
    DreamToolContext::getToolPropertiesWindow()
    {
        return &mToolPropertiesWindow;
    }

    MenuBar*
    DreamToolContext::getMenuBar()
    {
        return &mMenuBar;
    }

    RenderingDebugWindow*
    DreamToolContext::getRenderPipelineWindow()
    {
        return &mRenderPipelineWindow;
    }

    GamepadStateWindow*
    DreamToolContext::getGamepadStateWindow()
    {
        return &mGamepadStateWindow;
    }

    CacheContentWindow*
    DreamToolContext::getCacheContentWindow()
    {
        return &mCacheContentWindow;
    }

    TaskManagerWindow*
    DreamToolContext::getTaskManagerWindow()
    {
        return &mTaskManagerWindow;
    }

    GLPreviewWindowComponent*
    DreamToolContext::getGlPreviewWindowComponent()
    {
        return &mGlPreviewWindowComponent;
    }

    Grid*
    DreamToolContext::getGrid()
    {
        return &mGrid;
    }

    LightViewer*
    DreamToolContext::getLightViewer()
    {
        return &mLightViewer;
    }

    SelectionHighlighter*
    DreamToolContext::getSelectionHighlighter()
    {
        return &mSelectionHighlighter;
    }

    Cursor*
    DreamToolContext::getCursor()
    {
        return &mCursor;
    }

    PathViewer*
    DreamToolContext::getPathViewer()
    {
        return &mPathViewer;
    }

    AnimationViewer*
    DreamToolContext::getAnimationViewer()
    {
        return &mAnimationViewer;
    }

    // Data Models
    TemplatesModel*
    DreamToolContext::getTemplatesModel()
    {
        return &mTemplatesModel;
    }

    PreferencesModel*
    DreamToolContext::getPreferencesModel()
    {
        return &mPreferencesModel;
    }

    ModelDefinitionBatchImporter*
    DreamToolContext::getModelDefinitionBatchImporter()
    {
        return &mModelDefinitionBatchImporter;
    }

    ProjectDirectory*
    DreamToolContext::getProjectDirectory()
    {
        return &mProjectDirectory;
    }

    StorageManager*
    DreamToolContext::getStorageManager()
    {
        return &mStorageManager;
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


    bool DreamToolContext::openInExternalEditor(AssetDefinition* definition, const string& format)
    {
        if (mPreferencesModel.getExternalEditorPath().empty())
        {
            LOG_ERROR("DTContext: External editor has not been set");
            return false;
        }
        string filePath = getProjectDirectory()->getAssetAbsolutePath(definition,format);
        StorageManager* sm = getStorageManager();
        File* targetFile = sm->openFile(filePath);
        if (!targetFile->exists())
        {
        	LOG_DEBUG("DTContext: Attempted to open file that does not exist {}", targetFile->getPath());
           return false;
        }
        sm->closeFile(targetFile);
        targetFile = nullptr;

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

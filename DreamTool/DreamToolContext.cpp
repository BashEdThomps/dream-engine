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
          mAssetBrowser(this),
          mScriptDebugWindow(make_shared<ScriptDebugWindow>(this)),
          mSceneStateWindow(this),
          mToolPropertiesWindow(this),
          mMenuBar(this),
          mRenderPipelineWindow(this),
          mGamepadStateWindow(this),
          mCacheContentWindow(this),
          mTaskManagerWindow(this),
          mGlPreviewWindowComponent(make_shared<GLPreviewWindowComponent>(this)),
          // GL
          mGrid(this),
          mLightViewer(this),
          mSelectionHighlighter(this),
          mCursor(this),
          mPathViewer(this),
          mAnimationViewer(this),
          mPhysicsDebugDrawer(make_shared<PhysicsDebugDrawer>(this)),
          // Models
          mStorageManager(make_shared<StorageManager>()),
          mModelDefinitionBatchImporter(this),
          mTemplatesModel(this),
          mPreferencesModel(this),
          mAudioComponent(make_shared<OpenALAudioComponent>()),
          // Args
          mCommandArgumentsCount(_argc),
          mCommandArgumentsVector(_argv),
          mMainLoopDone(false),
          mInputTarget(INPUT_TARGET_EDITOR),
          mProjectBaseDirectory(nullptr)
    {
        LOG_TRACE("DTContext: Constructing");

        mProjectDirectory = make_shared<ProjectDirectory>(mStorageManager);
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
        mAudioComponent->init();

        // ImGui Widgets
        mWindow.addImGuiWidget(&mPropertiesWindow);
        mWindow.addImGuiWidget(&mProjectBrowser);
        mWindow.addImGuiWidget(&mAssetBrowser);
        mWindow.addImGuiWidget(mScriptDebugWindow.get());
        mWindow.addImGuiWidget(&mSceneStateWindow);
        mWindow.addImGuiWidget(&mMenuBar);
        mWindow.addImGuiWidget(&mToolPropertiesWindow);
        mWindow.addImGuiWidget(&mRenderPipelineWindow);
        mWindow.addImGuiWidget(&mGamepadStateWindow);
        mWindow.addImGuiWidget(&mCacheContentWindow);
        mWindow.addImGuiWidget(&mTaskManagerWindow);
        mGlPreviewWindowComponent->init();
        mWindow.addImGuiWidget(mGlPreviewWindowComponent.get());

        // GL Widgets

        mGrid.init();
        mGrid.setVisible(true);
        mWindow.addGLWidget(&mGrid);

        mCursor.init();
        mCursor.setVisible(true);
        mWindow.addGLWidget(&mCursor);

        mSelectionHighlighter.init();
        mWindow.addGLWidget(&mSelectionHighlighter);

        mPhysicsDebugDrawer->init();
        mPhysicsDebugDrawer->setVisible(true);
        mWindow.addGLWidget(mPhysicsDebugDrawer.get());

        mLightViewer.init();
        mLightViewer.setVisible(true);
        mWindow.addGLWidget(&mLightViewer);

        /*
        mPathViewer.init();
        mWindow.addGLWidget(&mPathViewer);

        mAnimationViewer.init();
        mWindow.addGLWidget(&mAnimationViewer);
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

            if (mProject)
            {
                auto projectRuntime = mProject->getRuntime();
                if (projectRuntime)
                {
                    projectRuntime->step();
                    mGlPreviewWindowComponent->updateWindow();
                }
            }

            mGlPreviewWindowComponent->bindFrameBuffer();
            mWindow.drawGLWidgets();

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
    (const shared_ptr<SceneRuntime>& sr)
    {
        ImGuiIO& io = ImGui::GetIO();

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
    (const shared_ptr<SceneRuntime>& sRunt)
    {
        ImGuiIO& io = ImGui::GetIO();
        auto pRunt = mProject->getRuntime();
        if (pRunt)
        {
            auto inputComp = pRunt->getInputComponent();
            if (inputComp)
            {
                auto ms = inputComp->getMouseState();
                auto ks = inputComp->getKeyboardState();
                auto js = inputComp->getJoystickState();

                // Mouse
                ms->setButtonsPressed(&io.MouseDown[0], 5);
                ms->setPosX(io.MousePos.x);
                ms->setPosY(io.MousePos.y);
                ms->setScrollX(DreamToolWindow::MouseWheelX);
                ms->setScrollY(DreamToolWindow::MouseWheelY);
                DreamToolWindow::MouseWheelX = 0.f;
                DreamToolWindow::MouseWheelY = 0.f;

                if (io.MouseWheel != 0.f || io.MouseWheelH != 0.f)
                {
                    LOG_ERROR("DTContext: Got some scroll X:{} Y:{} ",
                              io.MouseWheelH, io.MouseWheel);
                }

                // Keys
                ks->setKeysPressed(&io.KeysDown[0], 512);

                // Joystick
                int jsCount = 0;
                for (int id=GLFW_JOYSTICK_1; id < GLFW_JOYSTICK_LAST; id++)
                {
                    if (glfwJoystickPresent(id))
                    {
                        jsCount++;
                        js->setName(glfwGetJoystickName(id));
                        int numAxis, numButtons;
                        const float* axisData = glfwGetJoystickAxes(id,&numAxis);
                        const unsigned char* buttonData = glfwGetJoystickButtons(id, &numButtons);
                        if (axisData != nullptr)
                        {
                            js->setAxisCount(numAxis);
                            memcpy((void*)js->getAxisDataPointer(),axisData,sizeof(float)*numAxis);
                        }
                        else
                        {
                            js->setAxisCount(0);
                        }
                        if (buttonData != nullptr)
                        {
                            js->setButtonCount(numButtons);
                            memcpy((void*)js->getButtonDataPointer(),buttonData,sizeof(unsigned char)*numButtons);
                        }
                        else
                        {
                            js->setButtonCount(0);
                        }
                    }
                }

                inputComp->setKeyboardState(ks);
                inputComp->setMouseState(ms);
                inputComp->setJoystickState(js);
                inputComp->setJoystickCount(jsCount);
            }
        }
    }

    void
    DreamToolContext::closeProject
    ()
    {
        mPropertiesWindow.clearPropertyTargets();
        mProjectDirectory->closeProject();
        mProject.reset();
        if (mProjectBaseDirectory)
        {
            mStorageManager->closeDirectory(mProjectBaseDirectory);
        }
        mProjectBaseDirectory.reset();
    }

    bool
    DreamToolContext::openProject
    (const string& dir)
    {
        closeProject();

        LOG_DEBUG("DTContext: Opening project {}",dir);
        mLastDirectory = dir;
        mProjectBaseDirectory = mStorageManager->openDirectory(dir);
        mProject = mProjectDirectory->openFromDirectory(mProjectBaseDirectory);
        if(mProject)
        {
            mProject->setAudioComponent(mAudioComponent);
            mProject->setWindowComponent(mGlPreviewWindowComponent);
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
        mProjectBaseDirectory = mStorageManager->openDirectory(dir);
        mProject = mProjectDirectory->newProject(mProjectBaseDirectory);

        if(mProject)
        {
            mProject->setWindowComponent(mGlPreviewWindowComponent);
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

    shared_ptr<Project>
    DreamToolContext::getProject()
    const
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

    AssetBrowser*
    DreamToolContext::getAssetBrowser()
    {
        return &mAssetBrowser;
    }


    shared_ptr<ScriptDebugWindow>
    DreamToolContext::getScriptDebugWindow()
    const
    {
        return mScriptDebugWindow;
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

    shared_ptr<GLPreviewWindowComponent>
    DreamToolContext::getGlPreviewWindowComponent()
    const
    {
        return mGlPreviewWindowComponent;
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

    shared_ptr<ProjectDirectory>
    DreamToolContext::getProjectDirectory()
    const
    {
        return mProjectDirectory;
    }

    shared_ptr<StorageManager>
    DreamToolContext::getStorageManager()
    const
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
    (const shared_ptr<AssetDefinition>& definition, const string& format)
    {
        if (mPreferencesModel.getExternalEditorPath().empty())
        {
            LOG_ERROR("DTContext: External editor has not been set");
            return false;
        }
        string filePath = getProjectDirectory()->getAssetAbsolutePath(definition,format);
        auto sm = getStorageManager();
        auto targetFile = sm->openFile(filePath);
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

#include "DTContext.h"

using namespace Dream;

namespace DreamTool
{
    DTContext::DTContext(int _argc, char** _argv)
        :project(nullptr),
          // ImGui
          propertiesWindow(this),
          projectBrowser(this),
          scriptDebugWindow(this),
          sceneStateWindow(this),
          gridPropertiesWindow(this),
          scriptEditorWindow(this),
          shaderEditorWindow(this),
          menuBar(this),
          renderPipelineWindow(this),
          gamepadStateWindow(this),
          cacheContentWindow(this),
          taskManagerWindow(this),
          // GL
          grid(this),
          lightViewer(this),
          selectionHighlighter(this),
          cursor(this),
          pathViewer(this),
          animationViewer(this),
          modelDefinitionBatchImporter(this),
          // Args
          argc(_argc),
          argv(_argv)
    {
        LOG_TRACE("Constructing");
    }

    DTContext::~DTContext()
    {
        LOG_TRACE("Destructing");
    }

    bool DTContext::init()
    {
        #ifdef WIN32
        windowComponent.setUiFontSize(24.0f);
        windowComponent.setMonoFontSize(24.0f);
        #else
        windowComponent.setUiFontSize(16.0f);
        windowComponent.setMonoFontSize(18.0f);
        #endif
        if (!windowComponent.init())
        {
            return false;
        }

        ScriptComponent::AddPrintListener(&scriptDebugWindow);

        // ImGui Widgets
        windowComponent.addImGuiWidget(&propertiesWindow);
        windowComponent.addImGuiWidget(&projectBrowser);
        windowComponent.addImGuiWidget(&scriptDebugWindow);
        windowComponent.addImGuiWidget(&sceneStateWindow);
        windowComponent.addImGuiWidget(&menuBar);
        windowComponent.addImGuiWidget(&gridPropertiesWindow);
        windowComponent.addImGuiWidget(&scriptEditorWindow);
        windowComponent.addImGuiWidget(&shaderEditorWindow);
        windowComponent.addImGuiWidget(&renderPipelineWindow);
        windowComponent.addImGuiWidget(&gamepadStateWindow);
        windowComponent.addImGuiWidget(&cacheContentWindow);
        windowComponent.addImGuiWidget(&taskManagerWindow);

        // GL Widgets
        grid.init();
        lightViewer.init();
        selectionHighlighter.init();
        cursor.init();
        pathViewer.init();
        animationViewer.init();
        windowComponent.addGLWidget(&grid);
        windowComponent.addGLWidget(&lightViewer);
        windowComponent.addGLWidget(&selectionHighlighter);
        windowComponent.addGLWidget(&cursor);
        windowComponent.addGLWidget(&pathViewer);
        windowComponent.addGLWidget(&animationViewer);
        return true;
    }

    void DTContext::run()
    {
        if (argc > 1)
        {
            openProject(argv[1]);
        }

        // Run the project
        while (!MainLoopDone)
        {
            if (windowComponent.shouldClose())
            {
                MainLoopDone = true;
            }

            if (project && project->getRuntime())
            {
                auto projectRuntime = project->getRuntime();
                if(!projectRuntime->hasActiveScene())
                {
                    windowComponent.updateWindow(nullptr);
                    glClearColor(0.0f,0.0f,0.0f,0.0f);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                }
                projectRuntime->updateAll();
            }
            else
            {
                windowComponent.updateWindow(nullptr);
                glClearColor(0.0f,0.0f,0.0f,0.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            }

            ShaderRuntime::InvalidateState();
            windowComponent.drawGLWidgets();
            windowComponent.drawImGui();
            if (ImGui::IsKeyPressed(GLFW_KEY_TAB,false))
            {
                switch (inputTarget)
                {
                    case EDITOR:
                        inputTarget = SCENE;
                        break;
                    case SCENE:
                        inputTarget = EDITOR;
                        break;
                }
            }

            if (project && project->getRuntime())
            {
                auto sr = project->getRuntime()->getActiveSceneRuntime();
                if (sr)
                {
                    switch (inputTarget)
                    {
                        case InputTarget::EDITOR:
                            handleEditorInput(sr);
                            break;
                        case InputTarget::SCENE:
                            handleSceneInput(sr);
                            break;
                    }
                }
            }
            windowComponent.swapBuffers();
        }
    }

    void
    DTContext::handleEditorInput
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
                camera->deltaYaw(io.MouseDelta.x*mouseScalar);
                camera->deltaPitch(-io.MouseDelta.y*mouseScalar);
                sendKeysToCamera = true;
            }
            cursor.setMousePosition(io.MousePos.x, io.MousePos.y);
            #else
            if (io.MouseDown[2])
            {
                camera->deltaYaw(io.MouseDelta.x*mouseScalar);
                camera->deltaPitch(-io.MouseDelta.y*mouseScalar);
                sendKeysToCamera = true;
            }
            #endif
        }

        if (!io.WantCaptureKeyboard)
        {
            if (sendKeysToCamera)
            {
                if (ImGui::IsKeyDown(GLFW_KEY_W))
                {
                    camera->flyForward();
                }
                if (ImGui::IsKeyDown(GLFW_KEY_S))
                {
                    camera->flyBackward();
                }
                if (ImGui::IsKeyDown(GLFW_KEY_A))
                {
                    camera->flyLeft();
                }
                if (ImGui::IsKeyDown(GLFW_KEY_D))
                {
                    camera->flyRight();
                }
                if (ImGui::IsKeyDown(GLFW_KEY_Q))
                {
                    camera->flyDown();
                }
                if (ImGui::IsKeyDown(GLFW_KEY_E))
                {
                    camera->flyUp();
                }
            }
            // Send to cursor
            else
            {
                // Z
                if (ImGui::IsKeyPressed(GLFW_KEY_W))
                {
                    cursor.onAction(Cursor::ZPlus);
                }
                if (ImGui::IsKeyPressed(GLFW_KEY_S))
                {
                    cursor.onAction(Cursor::ZMinus);
                }

                // X
                if (ImGui::IsKeyPressed(GLFW_KEY_A))
                {
                    cursor.onAction(Cursor::XMinus);
                }
                if (ImGui::IsKeyPressed(GLFW_KEY_D))
                {
                    cursor.onAction(Cursor::XPlus);
                }

                // Y
                if (ImGui::IsKeyPressed(GLFW_KEY_Q))
                {
                    cursor.onAction(Cursor::YMinus);
                }
                if (ImGui::IsKeyPressed(GLFW_KEY_E))
                {
                    cursor.onAction(Cursor::YPlus);
                }
            }
        }
    }

    void
    DTContext::handleSceneInput
    (SceneRuntime* sRunt)
    {
        ImGuiIO& io = ImGui::GetIO();
        auto pRunt = project->getRuntime();
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
    DTContext::closeProject
    ()
    {
        propertiesWindow.clearPropertyTargets();
        projectDirectory.closeProject();
        project = nullptr;
    }

    bool
    DTContext::openProject
    (const string& dir)
    {
        closeProject();

        LOG_DEBUG("Opening project {}",dir);
        lastDirectory = dir;
        project = projectDirectory.openFromDirectory(dir);
        if(project)
        {
            project->setWindowComponent(&windowComponent);
            project->createProjectRuntime();
            stringstream ss;
            ss  << "Opened Project: "
                << project->getDefinition()->getName();
            menuBar.setMessageString(ss.str());
            return true;
        }
        return false;
    }

    bool
    DTContext::newProject
    (const string& dir)
    {
        LOG_DEBUG("Creating Project {}",dir);
        lastDirectory = dir;
        closeProject();
        project = projectDirectory.newProject(dir);
        if(project)
        {
            project->setWindowComponent(&windowComponent);
            project->createProjectRuntime();
            stringstream ss;
            ss  << "Created Project in: "
                << dir;
            menuBar.setMessageString(ss.str());
            return true;
        }
        else
        {
            stringstream ss;
            ss  << "Project already exists in this directory!";
            menuBar.setMessageString(ss.str());
        }
        return false;
    }
}

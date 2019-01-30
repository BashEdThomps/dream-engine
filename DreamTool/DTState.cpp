#include "DTState.h"
#include "../DreamCore/Scene/SceneRuntime.h"
#include "../DreamCore/Project/Project.h"
#include "../DreamCore/Project/ProjectDefinition.h"
#include "../DreamCore/Project/ProjectRuntime.h"
#include "../DreamCore/Components/Graphics/Shader/ShaderRuntime.h"
#include "../DreamCore/Components/Input/InputComponent.h"

using namespace Dream;

namespace DreamTool
{
    DTState::DTState(int _argc, char** _argv)
        : DreamObject("DTState"),
          project(nullptr),
          propertiesWindow(PropertiesWindow(this)),
          projectBrowser(ProjectBrowser(this)),
          luaDebugWindow(LuaDebugWindow(this)),
          sceneStateWindow(SceneStateWindow(this)),
          gridPropertiesWindow(ToolPropertiesWindow(this)),
          scriptEditorWindow(ScriptEditorWindow(this)),
          shaderEditorWindow(ShaderEditorWindow(this)),
          menuBar(MenuBar(this)),
          renderPipelineWindow(RenderingDebugWindow(this)),
          gamepadStateWindow(GamepadStateWindow(this)),
          grid(Grid(this)),
          lightViewer(LightViewer(this)),
          selectionHighlighter(SelectionHighlighter(this)),
          cursor(Cursor(this)),
          pathViewer(PathViewer(this)),
          animationViewer(AnimationViewer(this)),
          modelDefinitionBatchImporter(ModelDefinitionBatchImporter(this)),
          argc(_argc),
          argv(_argv)
    {
        #ifdef DREAM_LOG
        getLog()->trace("Constructing");
        #endif
    }

    DTState::~DTState()
    {
        #ifdef DREAM_LOG
        getLog()->trace("Destructing");
        #endif
    }

    void DTState::init()
    {
        #ifdef WIN32
        windowComponent.setUiFontSize(24.0f);
        windowComponent.setMonoFontSize(24.0f);
        #else
        windowComponent.setUiFontSize(16.0f);
        windowComponent.setMonoFontSize(18.0f);
        #endif
        windowComponent.init();

        ScriptComponent::AddPrintListener(&luaDebugWindow);

        // ImGui Widgets
        windowComponent.addImGuiWidget(&propertiesWindow);
        windowComponent.addImGuiWidget(&projectBrowser);
        windowComponent.addImGuiWidget(&luaDebugWindow);
        windowComponent.addImGuiWidget(&sceneStateWindow);
        windowComponent.addImGuiWidget(&menuBar);
        windowComponent.addImGuiWidget(&gridPropertiesWindow);
        windowComponent.addImGuiWidget(&scriptEditorWindow);
        windowComponent.addImGuiWidget(&shaderEditorWindow);
        windowComponent.addImGuiWidget(&renderPipelineWindow);
        windowComponent.addImGuiWidget(&gamepadStateWindow);

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
    }

    void DTState::run()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        #endif
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
                    windowComponent.updateComponent(nullptr);
                    glClearColor(0.0f,0.0f,0.0f,0.0f);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                }
                projectRuntime->updateAll();
            }
            else
            {
                windowComponent.updateComponent(nullptr);
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
            FPS();
        }
    }

    void DTState::FPS()
    {
        LastTime = CurrentTime;
        CurrentTime = glfwGetTime();
        Frames = 1.0/(CurrentTime-LastTime);
        menuBar.setFPS(Frames);
    }

    void
    DTState::handleEditorInput
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
    DTState::handleSceneInput
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
                memcpy(ms.ButtonsDown, io.MouseDown, sizeof(bool)*5);
                ms.PosX = io.MousePos.x;
                ms.PosY = io.MousePos.y;
                ms.ScrollX = io.MouseWheelH;
                ms.ScrollY = io.MouseWheel;

                // Keys
                memcpy(ks.KeysDown, io.KeysDown, sizeof(bool)*512);

                // Joystick
                for (int id=GLFW_JOYSTICK_1; id < GLFW_JOYSTICK_LAST; id++)
                {

                    if (glfwJoystickPresent(id))
                    {
                        js.Name = glfwGetJoystickName(id);
                        int numAxis, numButtons;
                        const float* axisData = glfwGetJoystickAxes(id,&numAxis);
                        const unsigned char* buttonData = glfwGetJoystickButtons(id, &numButtons);
                        if (axisData != nullptr)
                        {
                            js.AxisCount = numAxis;
                            memcpy(&js.AxisData[0],axisData,sizeof(float)*numAxis);
                        }
                        else
                        {
                            js.AxisCount = 0;
                        }
                        if (buttonData != nullptr)
                        {
                            js.ButtonCount = numButtons;
                            memcpy(&js.ButtonData[0],buttonData,sizeof(unsigned char)*numButtons);
                        }
                        else
                        {
                            js.ButtonCount = 0;
                        }
                    }
                }
            }
        }
    }

    void
    DTState::closeProject
    ()
    {
        propertiesWindow.clearPropertyTargets();
        projectDirectory.closeProject();
        project = nullptr;
    }

    bool
    DTState::openProject
    (string dir)
    {
        closeProject();

        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug("Opening project {}",dir);
        #endif
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
    DTState::newProject
    (string dir)
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug("Creating Project {}",dir);
        #endif
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

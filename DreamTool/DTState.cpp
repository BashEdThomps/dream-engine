#include "DTState.h"

namespace DreamTool
{
     DTState::DTState(int _argc, char** _argv)
         : DreamObject("DTState"),
           propertiesWindow(PropertiesWindow(this)),
           projectBrowser(ProjectBrowser(this)),
           luaDebugWindow(LuaDebugWindow(this)),
           sceneStateWindow(SceneStateWindow(this)),
           gridPropertiesWindow(GridPropertiesWindow(this)),
           scriptEditorWindow(ScriptEditorWindow(this)),
           menuBar(MenuBar(this)),
           grid(Grid(this)),
           lightViewer(LightViewer(this)),
           selectionHighlighter(SelectionHighlighter(this)),
           cursor(Cursor(this)),
           argc(_argc),
           argv(_argv)
     {

     }

    DTState::~DTState()
    {

    }

    void DTState::init()
    {
        project = new Dream::Project(&windowComponent);
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

        // GL Widgets
        grid.init();
        lightViewer.init();
        selectionHighlighter.init();
        cursor.init();
        windowComponent.addGLWidget(&grid);
        windowComponent.addGLWidget(&lightViewer);
        windowComponent.addGLWidget(&selectionHighlighter);
        windowComponent.addGLWidget(&cursor);
    }

    void DTState::run()
    {
        auto log = getLog();
        if (argc > 1)
        {
            if(project->openFromDirectory(argv[1]))
            {
                project->createProjectRuntime();
            }
        }

        spdlog::set_level(spdlog::level::err);
        // Run the project
        ImGuiIO& io = ImGui::GetIO();
        while (!MainLoopDone)
        {
            if (windowComponent.shouldClose())
            {
                MainLoopDone = true;
            }

            auto projectRuntime = project->getProjectRuntime();

            if (projectRuntime == nullptr)
            {
                windowComponent.updateComponent(nullptr);
                glClearColor(0.0f,0.0f,0.0f,0.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            }
            else if (projectRuntime)
            {
                if(!projectRuntime->hasActiveScene())
                {
                    windowComponent.updateComponent(nullptr);
                    glClearColor(0.0f,0.0f,0.0f,0.0f);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                }
                projectRuntime->updateAll();
            }

            Dream::ShaderInstance::InvalidateState();
            windowComponent.drawGLWidgets();
            windowComponent.drawImGui();
            if (projectRuntime)
            {
                auto sr = projectRuntime->getActiveSceneRuntime();
                if (sr)
                {
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
                                camera->flyForward(1.0f);
                            }
                            if (ImGui::IsKeyDown(GLFW_KEY_S))
                            {
                                camera->flyBackward(1.0f);
                            }
                            if (ImGui::IsKeyDown(GLFW_KEY_A))
                            {
                                camera->flyLeft(1.0f);
                            }
                            if (ImGui::IsKeyDown(GLFW_KEY_D))
                            {
                                camera->flyRight(1.0f);
                            }
                            if (ImGui::IsKeyDown(GLFW_KEY_Q))
                            {
                                camera->flyDown(1.0f);
                            }
                            if (ImGui::IsKeyDown(GLFW_KEY_E))
                            {
                                camera->flyUp(1.0f);
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
            }
            windowComponent.swapBuffers();
            if (CountFPS) showFPS();
            std::this_thread::yield();
        }
    }

    void DTState::showFPS()
    {
        if (glfwGetTime() > CurrentTime + OneSec)
        {
            cout << "FPS: " <<  Frames << endl;
            Frames = 0;
            CurrentTime = glfwGetTime();
        }
        else
        {
            Frames++;
        }
    }
}

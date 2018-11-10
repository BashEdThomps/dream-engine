#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <thread>
#include <memory>
#include "Window/DTWindowComponent.h"
#include <DreamCore.h>
// ImGui Widgets
#include "Widgets/ImGui/ProjectBrowser.h"
#include "Widgets/ImGui/PropertiesWindow.h"
#include "Widgets/ImGui/MenuBar.h"
#include "Widgets/ImGui/LuaDebugWindow.h"
#include "Widgets/ImGui/SceneStateWindow.h"
#include "Widgets/ImGui/GridPropertiesWidget.h"
// Plain GL Widgets
#include "Widgets/GL/Grid.h"
#include "Widgets/GL/LightViewer.h"
#include "Widgets/GL/SelectionHighlighterWidget.h"

#define MINIMUM_ARGUMENTS 3

using namespace std;
using namespace Dream;
using namespace DreamTool;

bool MainLoopDone = false;

static unsigned int Frames = 0;
static double CurrentTime = glfwGetTime();
static double OneSec = 1.0;
static bool CountFPS = false;

void
showFPS
()
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

int
main
(int argc,char** argv)
{
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("[%H:%M:%S|%n|%l] %v");

    auto log = spdlog::stdout_color_mt("Main");

    log->trace("Starting...");

    DTWindowComponent windowComponent;
#ifdef WIN32
    windowComponent.setUiFontSize(24.0f);
    windowComponent.setMonoFontSize(24.0f);
#else
    windowComponent.setUiFontSize(16.0f);
    windowComponent.setMonoFontSize(18.0f);
#endif
    windowComponent.init();

    // Widgets
    Dream::Project project(&windowComponent);
    PropertiesWindow propertiesWindow(&project);
    ProjectBrowser projectBrowser(&project, &propertiesWindow);
    LuaDebugWindow luaDebugWindow(&project);
    ScriptComponent::AddPrintListener(&luaDebugWindow);
    SceneStateWindow sceneStateWindow(&project);
    GridPropertiesWindow gridProps(&project);

    MenuBar menuBar
    (
        &project,
        &projectBrowser,
        &propertiesWindow,
        &luaDebugWindow,
        &sceneStateWindow,
        &gridProps
    );


    windowComponent.addImGuiWidget(&propertiesWindow);
    windowComponent.addImGuiWidget(&projectBrowser);
    windowComponent.addImGuiWidget(&luaDebugWindow);
    windowComponent.addImGuiWidget(&sceneStateWindow);
    windowComponent.addImGuiWidget(&menuBar);
    windowComponent.addImGuiWidget(&gridProps);

    Grid grid(&project);
    grid.init();
    gridProps.setGrid(&grid);

    LightViewer lv(&project);
    lv.init();

    SelectionHighlighterWidget sh(&project);
    sh.init();
    projectBrowser.setSelectionHighlighterWidget(&sh);
    propertiesWindow.setSelectionHighlighter(&sh);
	menuBar.setSelectionHighlighter(&sh);

    windowComponent.addGLWidget(&grid);
    windowComponent.addGLWidget(&lv);
    windowComponent.addGLWidget(&sh);

    if (argc > 1)
    {
       if(project.openFromDirectory(argv[1]))
       {
          project.createProjectRuntime();
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

        auto projectRuntime = project.getProjectRuntime();

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
                if (!io.WantCaptureMouse)
                {
#ifdef __APPLE__
                    if (io.MouseDown[0])
                    {
                        camera->deltaYaw(io.MouseDelta.x*mouseScalar);
                        camera->deltaPitch(-io.MouseDelta.y*mouseScalar);

                    }
#else
                    if (io.MouseDown[2])
                    {
                        camera->deltaYaw(io.MouseDelta.x*mouseScalar);
                        camera->deltaPitch(-io.MouseDelta.y*mouseScalar);
                    }
#endif
                }

                if (!io.WantCaptureKeyboard)
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
            }
        }
        windowComponent.swapBuffers();
        if (CountFPS) showFPS();
        std::this_thread::yield();
    }

    spdlog::set_level(spdlog::level::trace);
    log->info("Run is done. Performing stack-based clean up");
    return 0;
}

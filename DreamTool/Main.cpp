#include <iostream>
#include <thread>
#include <memory>
#include "Window/DTWindowComponent.h"
#include <DreamCore.h>
#include "Widgets/ImGui/ProjectBrowser.h"
#include "Widgets/ImGui/PropertiesWindow.h"
#include "Widgets/ImGui/MenuBar.h"
#include "Widgets/ImGui/LuaDebugWindow.h"
#include "Widgets/ImGui/SceneStateWindow.h"

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
(int,char**)
{
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("[%H:%M:%S|%n|%l] %v");

    auto log = spdlog::stdout_color_mt("Main");

    log->trace("Starting...");

    DTWindowComponent windowComponent;
    windowComponent.init();

    Project project(&windowComponent);
    PropertiesWindow propertiesWindow(&project);
    ProjectBrowser projectBrowser(&project, &propertiesWindow);
    LuaDebugWindow luaDebugWindow(&project);
    ScriptComponent::AddPrintListener(&luaDebugWindow);
    SceneStateWindow sceneStateWindow(&project);
    MenuBar menuBar
    (
        &project,
        &projectBrowser,
        &propertiesWindow,
        &luaDebugWindow,
        &sceneStateWindow
    );

    windowComponent.addImGuiWidget(&propertiesWindow);
    windowComponent.addImGuiWidget(&projectBrowser);
    windowComponent.addImGuiWidget(&luaDebugWindow);
    windowComponent.addImGuiWidget(&sceneStateWindow);
    windowComponent.addImGuiWidget(&menuBar);

    spdlog::set_level(spdlog::level::err);
     // Run the project
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
        windowComponent.drawImGui();
        windowComponent.swapBuffers();
        if (CountFPS) showFPS();
        std::this_thread::yield();
    }

    spdlog::set_level(spdlog::level::trace);
    log->info("Run is done. Performing stack-based clean up");
    return 0;
}

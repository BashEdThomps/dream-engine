#include <iostream>
#include <thread>
#include <memory>
#include "Window/DTWindowComponent.h"
#include <DreamCore.h>
#include "Widgets/ProjectBrowser.h"
#include "Widgets/PropertiesWindow.h"
#include "Widgets/MenuBar.h"
#include "Widgets/LuaDebugWindow.h"

#define MINIMUM_ARGUMENTS 3

using std::shared_ptr;
using std::unique_ptr;
using Dream::Constants;
using Dream::Project;
using Dream::SceneState;
using Dream::SceneDefinition;
using Dream::ArgumentParser;
using Dream::ProjectRuntime;
using Dream::ProjectDefinition;
using Dream::ScriptComponent;

using DreamTool::DTWindowComponent;
using DreamTool::ProjectBrowser;
using DreamTool::PropertiesWindow;
using DreamTool::MenuBar;
using DreamTool::LuaDebugWindow;

bool MainLoopDone = false;

void showUsage(const char** argv)
{
    cout << "Usage:" << endl
         << argv[0] << endl
         << "\t" << Constants::PROJECT_DIRECTORY_ARG << " <path/to/dream/project>" << endl
         << "\t" << Constants::PROJECT_UUID_ARG      << " <project_uuid>" << endl;
}

int main(int argc, const char** argv)
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
    MenuBar menuBar(&project,&projectBrowser,&propertiesWindow, &luaDebugWindow);

    windowComponent.addWidget(&propertiesWindow);
    windowComponent.addWidget(&projectBrowser);
    windowComponent.addWidget(&luaDebugWindow);
    windowComponent.addWidget(&menuBar);

    spdlog::set_level(spdlog::level::err);
     // Run the project
    unsigned int frames = 0;
    double time = glfwGetTime();
    double one_sec = 1.0;
    while (!MainLoopDone)
    {

        windowComponent.updateComponent(ProjectRuntime::CurrentSceneRuntime);
        if (windowComponent.shouldClose())
        {
            MainLoopDone = true;
        }

        if (ProjectRuntime::CurrentSceneRuntime != nullptr)
        {
            auto projectRuntime = project.getProjectRuntime();
            if (ProjectRuntime::CurrentSceneRuntime->getState() == SceneState::SCENE_STATE_STOPPED)
            {
                projectRuntime->destructSceneRuntime(ProjectRuntime::CurrentSceneRuntime);
                ProjectRuntime::CurrentSceneRuntime = nullptr;
            }
            else
            {
                projectRuntime->updateLogic(ProjectRuntime::CurrentSceneRuntime);
                projectRuntime->updateGraphics(ProjectRuntime::CurrentSceneRuntime);
                projectRuntime->collectGarbage(ProjectRuntime::CurrentSceneRuntime);
            }
        }
        else
        {
            glfwPollEvents();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        Dream::ShaderInstance::InvalidateState();
        windowComponent.drawImGui();
        windowComponent.swapBuffers();
        if (glfwGetTime() > time + one_sec)
        {
            //cout << "FPS: " <<  frames << endl;
            frames = 0;
            time = glfwGetTime();
        }
        else
        {
            frames++;
        }
        std::this_thread::yield();
    }

    spdlog::set_level(spdlog::level::trace);
    log->info("Run is done. Performing stack-based clean up");
    return 0;
}

#include <iostream>
#include <thread>
#include <memory>
#include "Window/DTWindowComponent.h"
#include <DreamCore.h>
#include "Tools/ProjectBrowser.h"
#include "Tools/AssetBrowser.h"
#include "Tools/PropertiesWindow.h"

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
using DreamTool::DTWindowComponent;
using DreamTool::ProjectBrowser;
using DreamTool::AssetBrowser;
using DreamTool::PropertiesWindow;

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

    DTWindowComponent windowComponent;
    Project project(&windowComponent);

    log->trace("Starting...");

    if (argc < MINIMUM_ARGUMENTS)
    {
        log->error("Minimum Number of Arguments Were Not Found.");
        showUsage(argv);
        return 1;
    }

    ArgumentParser parser(argc,argv);

    bool loaded = project.openFromArgumentParser(parser);

    if (!loaded)
    {
        log->error("Failed to Load Project.");
        return 1;
    }

    log->info("√ Definition Loading Complete... Creating Runtime");


    ProjectRuntime* pr = project.createProjectRuntime();

    if (pr == nullptr)
    {
        log->error("Unable to load project runtime");
        return 1;
    }

    ProjectDefinition* pd = project.getProjectDefinition();

    if (pd == nullptr)
    {
        log->error("Could not load project definition");
        return 1;
    }

    SceneDefinition* startupSceneDefinition  = pd->getStartupSceneDefinition();

    if (startupSceneDefinition == nullptr)
    {
        log->error("Error, could not find startup scene definition");
        return 1;
    }

    log->info("Using Startup Scene {}", startupSceneDefinition->getNameAndUuidString());

    auto sr = pr->constructActiveSceneRuntime(startupSceneDefinition);

    if (sr == nullptr)
    {
        log->error("Unable to create scene runtime, gotta go...");
        return -1;
    }

    ProjectBrowser projectBrowser(&project);
    windowComponent.addWidget(&projectBrowser);

    AssetBrowser assetBrowser(&project);
    windowComponent.addWidget(&assetBrowser);

    PropertiesWindow propertiesWindow(&project);
    windowComponent.addWidget(&propertiesWindow);

    spdlog::set_level(spdlog::level::err);
     // Run the project
    unsigned int frames = 0;
    double time = glfwGetTime();
    double one_sec = 1.0;
    while(sr->getState() != SceneState::SCENE_STATE_STOPPED)
    {
        pr->updateLogic();
        pr->updateGraphics();
        pr->collectGarbage();
        Dream::ShaderInstance::InvalidateState();
        windowComponent.drawImGui();
        windowComponent.swapBuffers();
        if (glfwGetTime() > time + one_sec)
        {
            cout << "FPS: " <<  frames << endl;
            frames = 0;
            time = glfwGetTime();
        }
        else
        {
            frames++;
        }
    }

    spdlog::set_level(spdlog::level::trace);
    log->info("Run is done. Performing stack-based clean up");
    return 0;
}

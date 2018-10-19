#include <iostream>
#include <thread>
#include <memory>
#include "Window/GLFWWindowComponent.h"
#include <DreamCore.h>

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
using DreamGLFW::GLFWWindowComponent;

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
    spdlog::set_pattern("[%H:%M:%S][%t][%n][%l] %v");

    auto log = spdlog::stdout_color_mt("Main");

    GLFWWindowComponent windowComponent;
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

    spdlog::set_level(spdlog::level::warn);

    // Run the project
    while(sr->getState() != SceneState::SCENE_STATE_STOPPED)
    {
        pr->updateLogic();
        pr->updateGraphics();
        pr->collectGarbage();
    }

    spdlog::set_level(spdlog::level::trace);
    log->info("Run is done. Performing stack-based clean up");
    return 0;
}

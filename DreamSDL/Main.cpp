#include <iostream>
#include <thread>
#include <memory>
#include "Window/SDLWindowComponent.h"
#include <DreamCore.h>

#define MINIMUM_ARGUMENTS 3

using std::shared_ptr;
using Dream::Constants;
using Dream::Project;
using Dream::SceneState;
using Dream::SceneDefinition;
using Dream::ArgumentParser;
using Dream::ProjectRuntime;
using Dream::ProjectDefinition;
using DreamSDL::SDLWindowComponent;

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

    shared_ptr<SDLWindowComponent> windowComponent = make_shared<SDLWindowComponent>();
    shared_ptr<Project> project = make_shared<Project>(windowComponent);

    log->trace("Starting...");

    if (argc < MINIMUM_ARGUMENTS)
    {
        log->error("Main: Minimum Number of Arguments Were Not Found.");
        showUsage(argv);
        return 1;
    }

    ArgumentParser parser(argc,argv);

    bool loaded = project->openFromArgumentParser(parser);

    if (!loaded)
    {
        log->error("Failed to Load Project.");
        return 1;
    }

    log->info("√ Definition Loading Complete... Creating Runtime");

    spdlog::set_level(spdlog::level::off);

    shared_ptr<ProjectRuntime> pr = project->createProjectRuntime();

    if (pr == nullptr)
    {
        log->error("Unable to lock project runtime");
        return 1;
    }

    shared_ptr<ProjectDefinition> pd = project->getProjectDefinition();

    if (pd == nullptr)
    {
        log->error("Could not lock project definition");
        return 1;
    }

    shared_ptr<SceneDefinition> startupSceneDefinition  = pd->getStartupSceneDefinition();

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

    // Run the project
    unsigned int frames = 0;
    unsigned int time = SDL_GetTicks();
    unsigned int one_sec = 1000;
    while(sr->getState() != SceneState::SCENE_STATE_STOPPED)
    {
        pr->updateLogic();
        pr->collectGarbage();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        pr->updateGraphics();

        if (SDL_GetTicks() > time + one_sec)
        {
            cout << "FPS: " <<  frames << endl;
            frames = 0;
            time = SDL_GetTicks();
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


#include <iostream>
#include <thread>
#include "include/DreamSDL.h"
#include <memory>

#include <DreamCore.h>

#define MINIMUM_ARGUMENTS 3

using std::cout;
using std::cerr;
using std::unique_ptr;
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
    {
        cout << "Usage:" << endl
             << argv[0] << endl
             << "\t" << Constants::PROJECT_DIRECTORY_ARG << " <path/to/dream/project>" << endl
             << "\t" << Constants::PROJECT_UUID_ARG      << " <project_uuid>" << endl;
    }
}

int main(int argc, const char** argv)
{

    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("[%H:%M:%S][%t][%n][%l] %v");
    auto log = spdlog::stdout_color_mt("Main");

    unique_ptr<SDLWindowComponent> windowComponent(new SDLWindowComponent());

    Project project(windowComponent.get());
    log->trace("Starting...");

    if (argc < MINIMUM_ARGUMENTS)
    {
        log->error("Main: Minimum Number of Arguments Were Not Found.");
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

    log->info(
         "          ==== Definition Loading Complete ====\n"
         "          ====       Creating Runtime      ====\n");

    spdlog::set_level(spdlog::level::off);

    ProjectRuntime* prHandle = project.createProjectRuntime();
    ProjectDefinition* pdHandle = project.getProjectDefinitionHandle();
    SceneDefinition* startupSceneDefinitionHandle = pdHandle->getStartupSceneDefinitionHandle();

    if (startupSceneDefinitionHandle == nullptr)
    {
        log->error("Error, could not find startup scene definition");
        return 1;
    }

    log->info("Using Startup Scene {}", startupSceneDefinitionHandle->getNameAndUuidString());

    SceneRuntime* srHandle = prHandle->constructActiveSceneRuntime(startupSceneDefinitionHandle);

    // Run the project
    unsigned int frames = 0;
    unsigned int time = SDL_GetTicks();
    unsigned int one_sec = 1000;
    while(srHandle->getState() != SceneState::SCENE_STATE_STOPPED)
    {
        prHandle->updateLogic();
        prHandle->collectGarbage();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        prHandle->updateGraphics();
        //std::this_thread::yield();

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
    log->info("          ====     Done. Stack-based cleanUp     ====");
    return 0;
}

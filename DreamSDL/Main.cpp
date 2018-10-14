/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <iostream>
#include <thread>
#include <memory>
#include "Window/SDLWindowComponent.h"
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

    SDLWindowComponent* windowComponent = new SDLWindowComponent();
    Project* project = new Project(windowComponent);

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


    ProjectRuntime* pr = project->createProjectRuntime();

    if (pr == nullptr)
    {
        log->error("Unable to lock project runtime");
        return 1;
    }

    ProjectDefinition* pd = project->getProjectDefinition();

    if (pd == nullptr)
    {
        log->error("Could not lock project definition");
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

    spdlog::set_level(spdlog::level::off);

    // Run the project
    unsigned int frames = 0;
    unsigned int time = SDL_GetTicks();
    unsigned int one_sec = 1000;
    while(sr->getState() != SceneState::SCENE_STATE_STOPPED)
    {
        pr->updateLogic();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        pr->updateGraphics();
        pr->collectGarbage();

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
    delete project;
    delete windowComponent;
    return 0;
}

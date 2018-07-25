
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
             << "\t" << Constants::PROJECT_DIRECTORY_ARG << " </home/username/.dreamtool>" << endl
             << "\t" << Constants::PROJECT_UUID_ARG      << " <project_uuid>" << endl;
    }
}

int main(int argc, const char** argv)
{

    unique_ptr<SDLWindowComponent> windowComponent(new SDLWindowComponent());

    Project project(windowComponent.get());


    {
        cout << "Main: Starting..." << endl;
    }

    if (argc < MINIMUM_ARGUMENTS)
    {
        cerr << "Main: Minimum Number of Arguments Were Not Found." << endl;
        showUsage(argv);
        return 1;
    }

    ArgumentParser parser(argc,argv);

    bool loaded = project.openFromArgumentParser(parser);

    if (!loaded)
    {
        cerr << "Main: Failed to Load Project." << endl;
        return 1;
    }

    cout << endl
         << "          ==== Definition Loading Complete ====" << endl
         << "          ====       Creating Runtime      ====" << endl
         << endl;

    ProjectRuntime* prHandle = project.createProjectRuntime();
    ProjectDefinition* pdHandle = project.getProjectDefinitionHandle();
    SceneDefinition* startupSceneDefinitionHandle = pdHandle->getStartupSceneDefinitionHandle();

    SceneRuntime* srHandle = prHandle->constructActiveSceneRuntime(startupSceneDefinitionHandle);

    //Constants::dreamSetVerbose(false);
    //Constants::dreamSetDebug(false);

    // Run the project
    while(srHandle->getState() != SceneState::SCENE_STATE_STOPPED)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        prHandle->updateAll();
        std::this_thread::yield();
    }

    cout << endl
         << "          ====     Done. Stack-based cleanUp     ====" << endl
         << endl;


    return 0;
}

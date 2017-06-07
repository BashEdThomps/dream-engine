#include <iostream>
#include <DreamCore.h>
#include "include/DreamSDL.h"
#include <thread>

#define MINIMUM_ARGUMENTS 3

using namespace std;
using namespace Dream;
using namespace DreamSDL;

void showUsage(const char** argv)
{
    if (DEBUG)
    {
        cout << "Usage:" << endl
             << argv[0] << endl
             << "\t" << PROJECT_DIRECTORY_ARG << " </home/username/.dreamtool>" << endl
             << "\t" << PROJECT_UUID_ARG      << " <project_uuid>" << endl;
    }
}

// Construct project with IWindowComponent
// Initialise Runtime
// loadFromSomePlace
// setActiveScene
// loadActiveScene

int main(int argc, const char** argv)
{
    Project *project = new Project(new SDLWindowComponent());

    dreamSetVerbose(true);

    if (DEBUG)
    {
        cout << "Main: Starting..." << endl;
    }

    if (argc < MINIMUM_ARGUMENTS)
    {
        cerr << "Main: Minimum Number of Arguments Were Not Found." << endl;
        showUsage(argv);
        return 1;
    }

    if(!project->initRuntime())
    {
        if (DEBUG)
        {
            cout << "Main: Bootstrapping Dream Failed" << endl;
        }
        return 1;
    }

    bool loaded = project->loadFromArgumentParser(new ArgumentParser(argc,argv));

    if (!loaded)
    {
        cerr << "Main: Failed to Load Project." << endl;
        return 1;
    }

    project->setStartupSceneActive();

    loaded = project->loadActiveScene();

    if (!loaded)
    {
        cerr << "Main: Error, unable to load startup scene" << endl;
        return 1;
    }

    dreamSetVerbose(false);
    dreamSetDebug(false);

    // Run the project
    while(project->getActiveScene()->getState() != SceneState::DONE)
    {
        project->updateAll();
        std::this_thread::yield();
    }

    dreamSetVerbose(true);

    project->cleanUpActiveScene();

    // Return the result
    return 0;
}

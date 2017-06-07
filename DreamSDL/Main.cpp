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
    if (Constants::DEBUG)
    {
        cout << "Usage:" << endl
             << argv[0] << endl
             << "\t" << Constants::PROJECT_DIRECTORY_ARG << " </home/username/.dreamtool>" << endl
             << "\t" << Constants::PROJECT_UUID_ARG      << " <project_uuid>" << endl;
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

    Constants::dreamSetVerbose(true);

    if (Constants::DEBUG)
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
        if (Constants::DEBUG)
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

    Constants::dreamSetVerbose(false);
    Constants::dreamSetDebug(false);

    // Run the project
    while(project->getActiveScene()->getState() != SceneState::DONE)
    {
        project->updateAll();
        std::this_thread::yield();
    }

    Constants::dreamSetVerbose(true);

    project->cleanUpActiveScene();

    // Return the result
    return 0;
}

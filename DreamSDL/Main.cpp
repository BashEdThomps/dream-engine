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

    loaded = project->loadStartupScene();

    if (!loaded)
    {
        cerr << "Main: Error, unable to load startup scene" << endl;
        return 1;
    }

    bool result = false;


    // Run the project
    while(!result)
    {
        result = project->updateAll();
        std::this_thread::yield();
    }

    // Return the result
    return result;
}

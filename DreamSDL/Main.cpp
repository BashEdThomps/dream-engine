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
    DreamEngine *engine = new Dream::DreamEngine(
                new SDLAudioComponent(),
                new SDLWindowComponent()
                );

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

    bool loaded = engine->loadFromArgumentParser(new Dream::ArgumentParser(argc,argv));

    if (!loaded)
    {
        cerr << "Main: Failed to Load Project." << endl;
        return 1;
    }


    if(!engine->initEngine())
    {
        if (DEBUG)
        {
            cout << "Main: Bootstrapping Dream Failed" << endl;
        }
        return 1;
    }

    bool result = false;


    // Run the engine
    while(!result)
    {
        result = engine->heartbeat();
        std::this_thread::yield();
    }

    // Return the result
    return result;
}

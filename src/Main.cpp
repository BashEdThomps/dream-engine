#include "DreamEngine.h"
#include "Lua/LuaEngine.h"
#include "ArgumentParser.h"
#include <iostream>
#include "Constants.h"

#define MINIMUM_ARGUMENTS 3

using namespace std;

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
    Dream::DreamEngine *engine = Dream::DreamEngine::getInstance();
    Dream::LuaEngine    lua;

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

    lua.init();

    int result = 0;
    while (result == 0)
    {
        lua.setLuaScriptMap(engine->getLuaScriptMap());

        if (!lua.createAllScripts())
        {
            cerr << "Main: While loading lua scripts" << endl;
            break;
        }

        lua.setSDL_Events(engine->getSDL_Events());

        if (!lua.update())
        {
            cerr << "Main: LuaComponentInstance update error!" << endl;
            result = 1;
            break;
        }

        if(!engine->update())
        {
            if (DEBUG)
            {
                cout << "Main: Dream Exited Cleanly" << endl;
            }
            result = 1;
            break;
        }
    }
    return result;
}

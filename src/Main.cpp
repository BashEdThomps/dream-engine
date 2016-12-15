#include "DreamEngine.h"
#include "Components/Scripting/LuaComponent.h"
#include "ArgumentParser.h"
#include <iostream>
#include "Constants.h"

#define VERBOSE

#define MINIMUM_ARGUMENTS 3

using namespace std;

void showUsage(const char** argv) {
  if (DEBUG) {
    cout << "Usage:" << endl
         << argv[0] << endl
         << "\t" << PROJECT_DIRECTORY_ARG << " </home/username/.dreamtool>" << endl
         << "\t" << PROJECT_UUID_ARG      << " <project_uuid>" << endl;
  }
}

int main(int argc, const char** argv) {
  Dream::DreamEngine *engine = Dream::DreamEngine::getInstance();
  Dream::LuaComponent lua;

  if (DEBUG) {
    cout << "Main:INFO:Starting..." << endl;
  }

  if (argc < MINIMUM_ARGUMENTS) {
    cerr << "Main:Error:Minimum Number of Arguments Were Not Found." << endl;
    showUsage(argv);
    return 1;
  }

  bool loaded = engine->loadFromArgumentParser(new Dream::ArgumentParser(argc,argv));

  if (!loaded) {
    cerr << "Main:Error:Failed to Load Project." << endl;
    return 1;
  }


  if(!engine->initEngine()) {

    if (DEBUG) {
      cout << "Main::Error:Bootstrapping Dream Failed" << endl;
    }
    return 1;
  }

  lua.setLuaScriptMap(engine->getLuaScriptMap());
  lua.init();

  int result = 0;
  while (result == 0) {

    if (!lua.loadScriptsFromMap()) {
      cerr << "Main:Error:While loading lua scripts" << endl;
      break;
    }

    lua.setSDL_Event(engine->getSDL_Event());

    if (!lua.update()) {
      cerr << "Main:Error:LuaComponentInstance update error!" << endl;
      result = 1;
      break;
    }

    if(!engine->update()) {
      cerr << "Main:Error:Dream update error!" << endl;
      result = 1;
      break;
    }
  }
  return result;
}

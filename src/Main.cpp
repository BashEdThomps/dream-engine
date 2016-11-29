#include "DreamEngine.h"
#include "Components/Scripting/LuaComponent.h"
#include "ArgumentParser.h"
#include <iostream>

#define VERBOSE

#define MINIMUM_ARGUMENTS 3

using namespace std;

Dream::DreamEngine DreamEngineInstance;
Dream::LuaComponent LuaComponentInstance;

void showUsage(const char** argv) {
  cout << "Usage:" << endl
            << argv[0] << endl
            << "\t" << PROJECT_DIRECTORY_ARG << " </home/username/.dreamtool> "
            << "\t" << PROJECT_UUID_ARG      << "<project_uuid>"
            << endl;
}

int main(int argc, const char** argv)
{
  cout << "Main: Starting..." << endl;

  if (argc < MINIMUM_ARGUMENTS)
  {
    cerr << "Main: FATAL - Minimum Number of Arguments Were Not Found." << endl;
    showUsage(argv);
    return 1;
  }

  bool loaded = DreamEngineInstance.loadFromArgumentParser(new Dream::ArgumentParser(argc,argv));

  if (loaded) {

    if(!DreamEngineInstance.bootstrap()) {
      cerr << "Main: Bootstrapping Dream Failed" << std::endl;
      return 1;
    }

    LuaComponentInstance.setLuaScriptMap(DreamEngineInstance.getLuaScriptMap());
    LuaComponentInstance.init();

    int result = 0;
    while (result == 0) {
        if (!LuaComponentInstance.update()) {
            cerr << "Main: LuaComponentInstance update error!" << endl;
            result = 1;
            break;
        }

        if(!DreamEngineInstance.update())
        {
            cerr << "Main: Dream update error!" << endl;
            result = 1;
            break;
        }
    }
    return 0;
  } else {
    cerr << "Main: FATAL - Failed to Load Project." << endl;
    return 1;
  }
}

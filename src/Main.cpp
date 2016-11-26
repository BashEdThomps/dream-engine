#include "Dream.h"
#include "Components/Scripting/ChaiScriptAPI.h"
#include "ArgumentParser.h"
#include <iostream>

#define VERBOSE

#define MINIMUM_ARGUMENTS 3

using namespace std;

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

  Dream::Dream dream;

  bool loaded = dream.loadFromArgumentParser(new Dream::ArgumentParser(argc,argv));

  if (loaded) {
    Dream::ChaiScriptAPI chaiAPI(&dream);
    if (!chaiAPI.init()) {
      cerr << "Main: Unable to initialise ChaiScriptAPI :(" << endl;
      return 1;
    }

    if(!dream.run())
    {
      cerr << "Main: Exiting Before It's Time :(" << endl;
      return 1;
    } else {
      cout << "Main: Exiting Cleanly :)" << endl;
      return 0;
    }
  } else {
    cerr << "Main: FATAL - Failed to Load Project." << endl;
    return 1;
  }
}

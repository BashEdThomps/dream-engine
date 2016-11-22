#include "Dream.h"
#include "Components/Scripting/ChaiScriptAPI.h"
#include "ArgumentParser.h"
#include <iostream>

#define VERBOSE

#define MINIMUM_ARGUMENTS 3

void showUsage(const char** argv) {
  std::cout << "Usage:" << std::endl
            << argv[0] << std::endl
            << "\t" << PROJECT_DIRECTORY_ARG << " </home/username/.dreamtool> "
            << "\t" << PROJECT_UUID_ARG      << "<project_uuid>"
            << std::endl;
}

int main(int argc, const char** argv)
{
  std::cout << "Main: Starting..." << std::endl;

  if (argc < MINIMUM_ARGUMENTS)
  {
    std::cerr << "Main: FATAL - Minimum Number of Arguments Were Not Found." << std::endl;
    showUsage(argv);
    return 1;
  }

  Dream::Dream dream;

  bool loaded = dream.loadFromArgumentParser(new Dream::ArgumentParser(argc,argv));

  if (loaded) {
    Dream::Components::Scripting::ChaiScriptAPI chaiAPI(&dream);
    if (!chaiAPI.init()) {
      std::cerr << "Main: Unable to initialise ChaiScriptAPI :(" << std::endl;
      return 1;
    }

    if(!dream.run())
    {
      std::cerr << "Main: Exiting Before It's Time :(" << std::endl;
      return 1;
    } else {
      std::cout << "Main: Exiting Cleanly :)" << std::endl;
      return 0;
    }
  } else {
    std::cerr << "Main: FATAL - Failed to Load Project." << std::endl;
    return 1;
  }
}

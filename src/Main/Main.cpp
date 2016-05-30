#include "Dream.h"
#include "ArgumentParser.h"
#include <iostream>

#define MINIMUM_ARGUMENTS 3

void showUsage(const char** argv) {
	std::cout << "Usage:" << std::endl
	          << argv[0] << std::endl
	          << "\t" << PROJECT_DIRECTORY_ARG << " </home/username/.dreamtool> "
						<< "\t" << PROJECT_UUID_ARG      << "<project_uuid>"
						<< std::endl;
}

int main(int argc, const char** argv) {
  std::cout << "Dream: Starting..." << std::endl;
	
	if (argc < MINIMUM_ARGUMENTS) {
		std::cerr << "Dream: FATAL - Minimum Number of Arguments Were Not Found." << std::endl;
		showUsage(argv);
		return 1;
	}
	
  Dream::Dream dream;
	
  bool loaded = dream.loadFromArgumentParser(new Dream::ArgumentParser(argc,argv));
	
  if (loaded) {
		if(!dream.runProject())
		{
			std::cerr << "Dream: Exiting Before It's Time :(" << std::endl;
			return 1;
		} else {
			std::cout << "Dream: Exiting Triumphantly >:D" << std::endl;
			return 0;
		}
	} else {
		std::cerr << "Dream: FATAL - Failed to Load Project." << std::endl;
		return 1;
	}
}

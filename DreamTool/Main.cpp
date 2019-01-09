#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <thread>
#include <memory>

#define MINIMUM_ARGUMENTS 3

#ifdef WIN32
#include <GL/glew.h>
#endif

#include "DTState.h"

using namespace DreamTool;

int
main
(int argc,char** argv)
{

#ifdef DREAM_LOG
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("[%H:%M:%S|%n|%l] %v");
    auto log = spdlog::stdout_color_mt("Main");
    log->trace("Starting...");
#endif
    DTState state(argc,argv);
    state.init();

#ifdef DREAM_LOG
    spdlog::set_level(spdlog::level::off);
#endif
    state.run();

#ifdef DREAM_LOG
    spdlog::set_level(spdlog::level::trace);
    log->info("Run is done. Performing stack-based clean up");
#endif
    return 0;
}

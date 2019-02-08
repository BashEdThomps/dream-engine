#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #include <GL/glew.h>
    #include <OpenGL/gl.h>
#endif

#ifdef WIN32
#include <windows.h>
#include <GL/glew.h>
#endif

#include <iostream>
#include <thread>
#include <memory>

#define MINIMUM_ARGUMENTS 3

#include "DTState.h"

using namespace DreamTool;

int
main
(int argc,char** argv)
{
    #ifdef DREAM_LOG
    spdlog::set_level(spdlog::level::off);
    spdlog::set_pattern("%H:%M:%S (%l) %n %v");
    auto log = spdlog::stdout_logger_mt("Main");
    log->trace("Starting...");
    #endif
    DTState state(argc,argv);
    if(!state.init())
    {
        return 1;
    }
    state.run();

    #ifdef DREAM_LOG
    log->info("Run is done. Performing stack-based clean up");
    #endif
    return 0;
}

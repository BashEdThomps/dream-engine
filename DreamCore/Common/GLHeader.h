#pragma once

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #include "glad/glad.h"
    #include <OpenGL/gl.h>
#endif

#ifdef __linux__
    #include "glad/glad.h"
    #include <GL/gl.h>
#endif

#ifdef _WIN32
    #ifndef NOMINMAX
        #define NOMINMAX // prevent windows redefining min/max
    #endif

    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif

    #include <windows.h>
    #include "glad/glad.h"
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif



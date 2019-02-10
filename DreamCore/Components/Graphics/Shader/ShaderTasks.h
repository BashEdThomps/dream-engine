#pragma once

#ifdef WIN32
#include <windows.h>
#include <GL/glew.h>
#include <GL/glu.h>
#endif

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <OpenGL/gl3.h>
//#include <GL/glu.h>
#endif

#ifdef __linux__
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

#include "../GraphicsComponentTask.h"

namespace Dream
{
    class ShaderRuntime;

    class ShaderCompileFragmentTask : public GraphicsComponentTask
    {
        ShaderRuntime* mShaderRuntime;
    public:
        ShaderCompileFragmentTask(ShaderRuntime* rt);
        void execute();
    };

    class ShaderCompileVertexTask : public GraphicsComponentTask
    {
        ShaderRuntime* mShaderRuntime;
    public:
        ShaderCompileVertexTask(ShaderRuntime* rt);
        void execute();
    };

    class ShaderLinkTask : public GraphicsComponentTask
    {
        ShaderRuntime* mShaderRuntime;
    public:
        ShaderLinkTask(ShaderRuntime* rt);
        void execute();
    };

    class ShaderFreeTask : public GraphicsComponentDestructionTask
    {
        GLuint mShaderProgram;
    public:
        ShaderFreeTask();
        void setShaderProgram(GLuint rt);
        void execute();
    };
}

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

// Defer GL Operations using Task objects
#include "../GraphicsComponentTask.h"

namespace Dream
{
    class TextureRuntime;
    class TextureConstructionTask : public GraphicsComponentTask
    {
        TextureRuntime* mTextureRuntime;
    public:
        TextureConstructionTask(TextureRuntime* rt);
        void execute();
    };

    class TextureDestructionTask : public GraphicsComponentDestructionTask
    {
        GLuint mTextureId;
    public:
        TextureDestructionTask();
        void setGLID(GLuint id);
        void execute() override;
    };
}

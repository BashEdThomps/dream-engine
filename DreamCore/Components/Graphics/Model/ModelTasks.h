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
    class ModelMesh;

    class ModelInitMeshTask : public GraphicsComponentTask
    {
        ModelMesh* mMesh;
    public:
        ModelInitMeshTask(ModelMesh* mesh);
        void execute();
    };

    class ModelFreeMeshTask : public GraphicsComponentTask
    {
        GLuint mVAO;
        GLuint mVBO;
        GLuint mIBO;
    public:
       ModelFreeMeshTask();
       void setBuffers(GLuint vao, GLuint vbo, GLuint ibo);
       void execute();
    };
}

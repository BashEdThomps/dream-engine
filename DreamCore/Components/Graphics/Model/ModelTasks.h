#pragma once

#include "ModelMesh.h"
#include "../GraphicsComponentTask.h"

namespace Dream
{

    class ModelInitMeshTask : public GraphicsComponentTask
    {
        ModelMesh* mMesh;
    public:
        ModelInitMeshTask(ModelMesh* mesh)
            : GraphicsComponentTask(),
              mMesh(mesh)
        {
            #ifdef DREAM_LOG
            setLogClassName("ModelInitMeshTask");
            #endif
        }

        inline void execute() override
        {
            #ifdef DREAM_LOG
            getLog()->critical("Executing on thread {}",mThreadId);
            #endif
            GLuint vao, vbo, ibo;
            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);
            glGenBuffers(1, &ibo);
            mMesh->setVAO(vao);
            mMesh->setVBO(vbo);
            mMesh->setIBO(ibo);

            glBindVertexArray(mMesh->getVAO());
            glBindBuffer(GL_ARRAY_BUFFER, mMesh->getVBO());
            glBufferData(GL_ARRAY_BUFFER, static_cast<GLint>(mMesh->getVertices().size() * sizeof(Vertex)), &mMesh->getVertices()[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mMesh->getIBO());
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLint>(mMesh->getIndices().size() * sizeof(GLuint)),&mMesh->getIndices()[0], GL_STATIC_DRAW);
            // Vertex Positions
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                static_cast<GLint>(sizeof(Vertex)), static_cast<GLvoid*>(nullptr));
            // Vertex Normals
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                static_cast<GLint>(sizeof(Vertex)),(GLvoid*)offsetof(Vertex, Normal));
            // Vertex Texture Coords
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                static_cast<GLint>(sizeof(Vertex)),(GLvoid*)offsetof(Vertex, TexCoords));
            // Vertex Tangents
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,
                static_cast<GLint>(sizeof(Vertex)),(GLvoid*)offsetof(Vertex, Tangent));
            // Vertex Bitangents
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE,
                static_cast<GLint>(sizeof(Vertex)),(GLvoid*)offsetof(Vertex, Bitangent));
            glBindVertexArray(0);
            mMesh->clearInitMeshTask();
        }
    };

    class ModelFreeMeshTask : public GraphicsComponentTask
    {
        GLuint mVAO;
        GLuint mVBO;
        GLuint mIBO;
    public:
       ModelFreeMeshTask
       (GLuint vao, GLuint vbo, GLuint ibo)
           : GraphicsComponentTask (),
             mVAO(vao),
             mVBO(vbo),
             mIBO(ibo)
       {
           #ifdef DREAM_LOG
           setLogClassName("ModelFreeMeshTask");
           #endif
       }

       inline void execute()
       {
           #ifdef DREAM_LOG
           getLog()->critical("Executing on thread {}",mThreadId);
           #endif

            if (mVAO > 0)
            {
                glDeleteVertexArrays(1,&mVAO);
            }

            if (mVBO > 0)
            {
                glDeleteBuffers(1,&mVBO);
            }

            if (mIBO > 0)
            {
                glDeleteBuffers(1,&mIBO);
            }
       }
    };
}

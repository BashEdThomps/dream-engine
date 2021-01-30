#include "ShaderRuntime.h"
#include "ShaderTasks.h"
#include "Components/Graphics/GraphicsComponent.h"

#define ERROR_BUF_SZ 4096
namespace octronic::dream
{

    ShaderCompileFragmentTask::ShaderCompileFragmentTask(ShaderRuntime* rt)
        : GraphicsComponentTask("ShaderCompileFragmentTask"), mShaderRuntime(rt)
    {
    }

    void ShaderCompileFragmentTask::execute()
    {
        mShaderRuntime->lock();
        LOG_TRACE("ShaderCompileFragmentTask: {} Executing on Graphics thread",mShaderRuntime->getName());
        GLint success;
        GLchar infoLog[ERROR_BUF_SZ];
        // Fragment Shader
        mShaderRuntime->setFragmentShader(glCreateShader(GL_FRAGMENT_SHADER));
        string fs = mShaderRuntime->getFragmentSource().c_str();

        const char *fSource = fs.c_str();
        glShaderSource(mShaderRuntime->getFragmentShader(), 1, &fSource, nullptr);
        glCompileShader(mShaderRuntime->getFragmentShader());
        // Print compile errors if any
        glGetShaderiv(mShaderRuntime->getFragmentShader(), GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(mShaderRuntime->getFragmentShader(), ERROR_BUF_SZ, nullptr, infoLog);
            LOG_CRITICAL( "ShaderCompileVertexTask:\n"
                       "\tFRAGMENT SHADER COMPILATION FAILED\n"
                       "\tShaderRuntime: {}\n"
                       "\t{}",
                       mShaderRuntime->getNameAndUuidString(),
                       infoLog );
            glDeleteShader(mShaderRuntime->getFragmentShader());
            mShaderRuntime->setFragmentShader(0);
            mShaderRuntime->unlock();
            return;
        }
        setState(TASK_STATE_COMPLETED);
        mShaderRuntime->unlock();
    }

    ShaderCompileVertexTask::ShaderCompileVertexTask(ShaderRuntime* rt)
        : GraphicsComponentTask("ShaderCompileVertexTask"), mShaderRuntime(rt)
    {
    }

    void ShaderCompileVertexTask::execute()
    {
        mShaderRuntime->lock();
        LOG_TRACE("ShaderCompileVertexTask: {} Executing on Graphics thread",mShaderRuntime->getName());
        GLint success;
        GLchar infoLog[ERROR_BUF_SZ];
        // Vertex Shader
        mShaderRuntime->setVertexShader(glCreateShader(GL_VERTEX_SHADER));
        string vs = mShaderRuntime->getVertexSource().c_str();

        const char *vSource = vs.c_str();
        glShaderSource(mShaderRuntime->getVertexShader(), 1, &vSource, nullptr);
        glCompileShader(mShaderRuntime->getVertexShader());
        // Print compile errors if any
        glGetShaderiv(mShaderRuntime->getVertexShader(), GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(mShaderRuntime->getVertexShader(), ERROR_BUF_SZ, nullptr, infoLog);
            LOG_CRITICAL( "ShaderCompileVertexTask:\n"
                       "\tVERTEX SHADER COMPILATION FAILED\n"
                       "\tShaderRuntime: {}\n"
                       "\t{}",
                       mShaderRuntime->getNameAndUuidString(),
                       infoLog );
            glDeleteShader(mShaderRuntime->getVertexShader());
            mShaderRuntime->setVertexShader(0);
            mShaderRuntime->unlock();
            return;
        }
        setState(TASK_STATE_COMPLETED);
        mShaderRuntime->unlock();
    }

    ShaderLinkTask::ShaderLinkTask(ShaderRuntime* rt)
        : GraphicsComponentTask("ShaderLinkTask"), mShaderRuntime(rt)
    {
    }

    void ShaderLinkTask::execute()
    {
        LOG_TRACE("ShaderLinkTask: Linking Shader [{}] on Graphics thread",mShaderRuntime->getName());
        mShaderRuntime->lock();
        if (mShaderRuntime->getVertexShader() != 0 && mShaderRuntime->getFragmentShader() != 0)
        {
            GLint success;

            // Create Shader Program
            mShaderRuntime->setShaderProgram(glCreateProgram());
            if (mShaderRuntime->getShaderProgram() == 0)
            {
                LOG_CRITICAL( "ShaderLinkTask:\n"
                           "\tSHADER LINKING FAILED\n"
                           "\tShaderRuntime: {}\n",
                           mShaderRuntime->getNameAndUuidString());

                mShaderRuntime->unlock();
                return;
            }

            glAttachShader(mShaderRuntime->getShaderProgram(), mShaderRuntime->getVertexShader());
            glAttachShader(mShaderRuntime->getShaderProgram(), mShaderRuntime->getFragmentShader());
            glLinkProgram(mShaderRuntime->getShaderProgram());

            // Print linking errors if any
            glGetProgramiv(mShaderRuntime->getShaderProgram(), GL_LINK_STATUS, &success);
            GLchar infoLog[ERROR_BUF_SZ];
            if (!success)
            {
                glGetProgramInfoLog(mShaderRuntime->getShaderProgram(), ERROR_BUF_SZ, nullptr, infoLog);
                LOG_CRITICAL("ShaderLinkTask: SHADER PROGRAM LINKING FAILED\n {}" , infoLog );
                glDeleteProgram(mShaderRuntime->getShaderProgram());
                mShaderRuntime->setShaderProgram(0);
                mShaderRuntime->unlock();
                return;
            }

            // Delete the shaders as they're linked into our program now and no longer necessery
            glDeleteShader(mShaderRuntime->getVertexShader());
            glDeleteShader(mShaderRuntime->getFragmentShader());

            mShaderRuntime->setLoaded(mShaderRuntime->getShaderProgram() != 0);

            if (mShaderRuntime->getLoaded())
            {
                mShaderRuntime->setPointLightCountLocation(
                            glGetUniformLocation(mShaderRuntime->getShaderProgram(),
                                                 ShaderRuntime::UNIFORM_POINT_LIGHT_COUNT));

                mShaderRuntime->setSpotLightCountLocation(
                            glGetUniformLocation(mShaderRuntime->getShaderProgram(),
                                                 ShaderRuntime::UNIFORM_SPOT_LIGHT_COUNT));

                mShaderRuntime->setDirectionalLightCountLocation(
                            glGetUniformLocation(mShaderRuntime->getShaderProgram(),
                                                 ShaderRuntime::UNIFORM_DIRECTIONAL_LIGHT_COUNT));
            }
        }
        setState(TASK_STATE_COMPLETED);
        mShaderRuntime->unlock();
    }

    ShaderFreeTask::ShaderFreeTask()
        : GraphicsComponentDestructionTask("ShaderFreeTask"),
          mShaderProgram(0)
    {
    }

    void ShaderFreeTask::setShaderProgram(GLuint rt)
    {
        mShaderProgram = rt;
    }

    void ShaderFreeTask::execute()
    {
        LOG_TRACE("ShaderFreeTask: ShaderProgram:{} Executing on Graphics thread",mShaderProgram);
        glDeleteProgram(mShaderProgram);
        setState(TASK_STATE_COMPLETED);
    }
}

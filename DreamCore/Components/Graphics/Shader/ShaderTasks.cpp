#include "ShaderRuntime.h"
#include "ShaderTasks.h"

namespace Dream
{

    ShaderCompileFragmentTask::ShaderCompileFragmentTask(ShaderRuntime* rt)
        : GraphicsComponentTask(), mShaderRuntime(rt)
    {
    }

    void ShaderCompileFragmentTask::execute()
    {
        mShaderRuntime->lock();
        LOG_CRITICAL("Executing on thread {}",mThreadId);
        GLint success;
        GLchar infoLog[512];
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
            glGetShaderInfoLog(mShaderRuntime->getFragmentShader(), 512, nullptr, infoLog);
            LOG_ERROR( "FRAGMENT SHADER COMPILATION FAILED\n {}" , infoLog );
            glDeleteShader(mShaderRuntime->getFragmentShader());
            mShaderRuntime->setFragmentShader(0);
            mShaderRuntime->unlock();
            return;
        }
        setState(COMPLETED);
        mShaderRuntime->unlock();
    }

    ShaderCompileVertexTask::ShaderCompileVertexTask(ShaderRuntime* rt)
        : GraphicsComponentTask(), mShaderRuntime(rt)
    {
    }

    void ShaderCompileVertexTask::execute()
    {
        mShaderRuntime->lock();
        LOG_CRITICAL("Executing on thread {}",mThreadId);
        GLint success;
        GLchar infoLog[512];
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
            glGetShaderInfoLog(mShaderRuntime->getVertexShader(), 512, nullptr, infoLog);
            LOG_ERROR( "VERTEX SHADER COMPILATION FAILED\n {}" ,infoLog );
            glDeleteShader(mShaderRuntime->getVertexShader());
            mShaderRuntime->setVertexShader(0);
            mShaderRuntime->unlock();
            return;
        }
        setState(COMPLETED);
        mShaderRuntime->unlock();
    }

    ShaderLinkTask::ShaderLinkTask(ShaderRuntime* rt)
        : GraphicsComponentTask(), mShaderRuntime(rt)
    {
    }

    void ShaderLinkTask::execute()
    {
        LOG_CRITICAL("Executing on thread {}",mThreadId);
        mShaderRuntime->lock();
        if (mShaderRuntime->getVertexShader() != 0 && mShaderRuntime->getFragmentShader() != 0)
        {
            GLint success;

            // Create Shader Program
            mShaderRuntime->setShaderProgram(glCreateProgram());
            if (mShaderRuntime->getShaderProgram() == 0)
            {
                LOG_ERROR("Unable to create shader program");
                mShaderRuntime->unlock();
                return;
            }

            glAttachShader(mShaderRuntime->getShaderProgram(), mShaderRuntime->getVertexShader());
            glAttachShader(mShaderRuntime->getShaderProgram(), mShaderRuntime->getFragmentShader());
            glLinkProgram(mShaderRuntime->getShaderProgram());

            // Print linking errors if any
            glGetProgramiv(mShaderRuntime->getShaderProgram(), GL_LINK_STATUS, &success);
            GLchar infoLog[512];
            if (!success)
            {
                glGetProgramInfoLog(mShaderRuntime->getShaderProgram(), 512, nullptr, infoLog);
                LOG_ERROR( "SHADER PROGRAM LINKING FAILED\n {}" , infoLog );
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
        setState(COMPLETED);
        mShaderRuntime->unlock();
    }

    ShaderFreeTask::ShaderFreeTask()
        : GraphicsComponentDestructionTask(), mShaderProgram(0)
    {
    }

    void ShaderFreeTask::setShaderProgram(GLuint rt)
    {
        mShaderProgram = rt;
    }

    void ShaderFreeTask::execute()
    {
        LOG_CRITICAL("Executing on thread {}",mThreadId);
        glDeleteProgram(mShaderProgram);
        setState(COMPLETED);
    }
}

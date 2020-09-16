#pragma once

#include "../../../Common/GLHeader.h"
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
        void execute() override;
    };
}

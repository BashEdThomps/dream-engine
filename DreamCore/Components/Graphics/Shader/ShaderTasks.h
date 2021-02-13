#pragma once

#include "Common/GLHeader.h"
#include "Components/Graphics/GraphicsComponentTasks.h"

namespace octronic::dream
{
    class ShaderRuntime;

    class ShaderCompileFragmentTask : public GraphicsTask
    {
        ShaderRuntime* mShaderRuntime;
    public:
        ShaderCompileFragmentTask(ProjectRuntime* prt, ShaderRuntime* rt);
        void execute();
    };

    class ShaderCompileVertexTask : public GraphicsTask
    {
        ShaderRuntime* mShaderRuntime;
    public:
        ShaderCompileVertexTask(ProjectRuntime* prt, ShaderRuntime* rt);
        void execute();
    };

    class ShaderLinkTask : public GraphicsTask
    {
        ShaderRuntime* mShaderRuntime;
    public:
        ShaderLinkTask(ProjectRuntime* prt, ShaderRuntime* rt);
        void execute();
    };

    class ShaderFreeTask : public GraphicsDestructionTask
    {
        GLuint mShaderProgram;
    public:
        ShaderFreeTask(ProjectRuntime* prt);
        void setShaderProgram(GLuint rt);
        void execute() override;
    };
}

#pragma once

#include "Common/GLHeader.h"
#include "Components/Graphics/GraphicsComponentTasks.h"

namespace octronic::dream
{
    class ShaderRuntime;

    class ShaderCompileFragmentTask : public GraphicsTask
    {
        weak_ptr<ShaderRuntime> mShaderRuntime;
    public:
        ShaderCompileFragmentTask(const weak_ptr<ProjectRuntime>& prt,
                                  const weak_ptr<ShaderRuntime>& rt);
        void execute();
    };

    class ShaderCompileVertexTask : public GraphicsTask
    {
        const weak_ptr<ShaderRuntime>& mShaderRuntime;
    public:
        ShaderCompileVertexTask(const weak_ptr<ProjectRuntime>& prt,
                                const weak_ptr<ShaderRuntime>& rt);
        void execute();
    };

    class ShaderLinkTask : public GraphicsTask
    {
        weak_ptr<ShaderRuntime> mShaderRuntime;
    public:
        ShaderLinkTask(const weak_ptr<ProjectRuntime>& prt,
                       const weak_ptr<ShaderRuntime>& rt);
        void execute();
    };

    class ShaderFreeTask : public GraphicsDestructionTask
    {
        GLuint mShaderProgram;
    public:
        ShaderFreeTask(const weak_ptr<ProjectRuntime>& prt);
        void setShaderProgram(GLuint rt);
        void execute() override;
    };
}

#pragma once

#include "Common/GLHeader.h"
#include "Components/Graphics/GraphicsComponentTasks.h"

namespace octronic::dream
{
  class ShaderRuntime;

  // ShaderRuntimeTask =========================================================

  class ShaderRuntimeTask : public GraphicsTask
  {
  public:
    ShaderRuntimeTask(ProjectRuntime&, ShaderRuntime&, const string&);
  protected:
    ShaderRuntime& getShaderRuntime() const;
  private:
    reference_wrapper<ShaderRuntime> mShaderRuntime;
  };

  // ShaderCompileFragmentTask =================================================

  class ShaderCompileFragmentTask : public ShaderRuntimeTask
  {
  public:
    ShaderCompileFragmentTask(ProjectRuntime& prt,ShaderRuntime& rt);
    void execute();
  };

  // ShaderCompileVertexTask ===================================================

  class ShaderCompileVertexTask : public ShaderRuntimeTask
  {
  public:
    ShaderCompileVertexTask(ProjectRuntime& prt,ShaderRuntime& rt);
    void execute();
  };

  // ShaderLinkTask ============================================================

  class ShaderLinkTask : public ShaderRuntimeTask
  {
  public:
    ShaderLinkTask(ProjectRuntime& prt,ShaderRuntime& rt);
    void execute();
  };

  // ShaderFreeTask ============================================================

  class ShaderFreeTask : public GraphicsDestructionTask
  {
    GLuint mShaderProgram;
  public:
    ShaderFreeTask(ProjectRuntime& prt);
    void setShaderProgram(GLuint rt);
    void execute() override;
  };
}

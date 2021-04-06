#include "TaskManagerWindow.h"
#include "DreamToolContext.h"
#include <DreamCore.h>

namespace octronic::dream::tool
{
  TaskManagerWindow::TaskManagerWindow
  (DreamToolContext& proj) : ImGuiWidget(proj,false)
  {
  }

  TaskManagerWindow::~TaskManagerWindow
  ()
  {

  }

  void
  TaskManagerWindow::draw
  ()
  {
    ImGui::Begin("Task Manager",&mVisible);

    auto& ctx = getContext();
    auto& pCtxOpt = ctx.getProjectContext();
    if (pCtxOpt)
    {
      auto& pCtx = pCtxOpt.value();
      auto& pRuntOpt = pCtx.getProjectRuntime();

      if (pRuntOpt)
      {
      }
      else
      {
        ImGui::Text("No Project Runtime");
      }
    }
    else
    {
      ImGui::Text("No Project");
    }
    ImGui::End();
  }
}

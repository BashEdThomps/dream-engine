#include "RenderingDebugWindow.h"

#include "DreamToolContext.h"

#include <DreamCore.h>

using octronic::dream::Project;
using octronic::dream::ProjectRuntime;

namespace octronic::dream::tool
{
  RenderingDebugWindow::RenderingDebugWindow
  (DreamToolContext& state)
    : ImGuiWidget(state,false)
  {
  }

  void
  RenderingDebugWindow::draw
  ()
  {
    auto& ctx = getContext();
    auto& pCtxOpt = ctx.getProjectContext();

    if (pCtxOpt)
    {
      auto& pCtx = pCtxOpt.value();

      ImGui::Begin("Render Pipeline",&mVisible);

      auto& pRuntOpt = pCtx.getProjectRuntime();

      if (pRuntOpt)
      {
        auto& pRunt = pRuntOpt.value();

        auto& gfx = pRunt.getGraphicsComponent();

        if (ImGui::CollapsingHeader("Statistics"))
        {
          ImGui::PushItemWidth(-1);
          ImGui::PlotLines("FPS",&pRunt.getFrameDurationHistory()[0],
              ProjectRuntime::MaxFrameCount, 0, "FPS", 0.f, 250.f, ImVec2(0,50));
          ImGui::PopItemWidth();
          ImGui::Text("Average: %.3f",pRunt.getAverageFramerate());
          ImGui::Separator();
          ImGui::Text("Geometry Triangles Drawn: %ld", ModelMesh::TrianglesDrawn);
          ImGui::Text("Geometry Meshes Drawn: %ld", ModelMesh::MeshesDrawn);
          ImGui::Text("Geometry Draw Calls: %ld", ModelMesh::DrawCalls);
          ImGui::Separator();
          ImGui::Text("Shadow Triangles Drawn: %ld", ModelMesh::ShadowTrianglesDrawn);
          ImGui::Text("Shadow Meshes Drawn: %ld", ModelMesh::ShadowMeshesDrawn);
          ImGui::Text("Shadow Draw Calls: %ld", ModelMesh::ShadowDrawCalls);
        }

        if(ImGui::CollapsingHeader("Shadow Pass"))
        {
          ImVec2 ca = ImGui::GetContentRegionAvail();
          ImVec2 img_size(ca.y, ca.y);
          ImGui::Text("Depth");
          ImGui::Image((void*)(intptr_t)gfx.getShadowPassDepthBuffer(),img_size,UV1,UV2);
        }
      }
      ImGui::End();
    }
  }

  ImVec2 RenderingDebugWindow::UV1 = ImVec2(0,1);
  ImVec2 RenderingDebugWindow::UV2 = ImVec2(1,0);
}

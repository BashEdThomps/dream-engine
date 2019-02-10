#include "RenderingDebugWindow.h"
#include "../../DTState.h"
#include "../../../DreamCore/Project/Project.h"
#include "../../../DreamCore/Project/ProjectRuntime.h"
#include "../../../DreamCore/Components/Graphics/GraphicsComponent.h"
#include "../../../DreamCore/Components/Graphics/Model/ModelMesh.h"

using namespace Dream;

namespace DreamTool
{
    RenderingDebugWindow::RenderingDebugWindow
    (DTState* state)
        :ImGuiWidget(state,false)
    {
#ifdef DREAM_LOG
        setLogClassName("RenderingDebugWindow");
#endif
    }

    RenderingDebugWindow::~RenderingDebugWindow (){}


    void
    RenderingDebugWindow::draw
    ()
    {
        if (mState->project)
        {
            ImGui::Begin("Render Pipeline",&mVisible);
            auto pRuntime = mState->project->getRuntime();
            if (pRuntime)
            {
                auto gfx = pRuntime->getGraphicsComponent();
                if (gfx)
                {
                    if (ImGui::CollapsingHeader("Statistics"))
                    {

                        ImGui::PushItemWidth(-1);
                        ImGui::PlotLines("FPS",&pRuntime->getFrameDurationHistory()[0],ProjectRuntime::MaxFrameCount, 0, "FPS", 0.f, 250.f, ImVec2(0,50));
                        ImGui::PopItemWidth();
                        ImGui::Text("Average: %.3f",pRuntime->getAverageFramerate());
                        ImGui::Separator();
                        ImGui::Text("Geometry Triangles Drawn: %ld", ModelMesh::TrianglesDrawn);
                        ImGui::Text("Geometry Meshes Drawn: %ld", ModelMesh::MeshesDrawn);
                        ImGui::Text("Geometry Draw Calls: %ld", ModelMesh::DrawCalls);
                        ImGui::Separator();
                        ImGui::Text("Shadow Triangles Drawn: %ld", ModelMesh::ShadowTrianglesDrawn);
                        ImGui::Text("Shadow Meshes Drawn: %ld", ModelMesh::ShadowMeshesDrawn);
                        ImGui::Text("Shadow Draw Calls: %ld", ModelMesh::ShadowDrawCalls);
                    }

                    if (ImGui::CollapsingHeader("Geometry Pass"))
                    {

                        ImGui::Columns(4);

                        ImGui::Text("Albedo");
                        ImGui::Image((void*)(intptr_t)gfx->getGeometryPassAlbedoBuffer(),PreviewSize, UV1, UV2);
                        ImGui::NextColumn();

                        ImGui::Text("Position");
                        ImGui::Image((void*)(intptr_t)gfx->getGeometryPassPositionBuffer(),PreviewSize, UV1, UV2);
                        ImGui::NextColumn();

                        ImGui::Text("Normal");
                        ImGui::Image((void*)(intptr_t)gfx->getGeometryPassNormalBuffer(),PreviewSize, UV1, UV2);
                        ImGui::NextColumn();

                        ImGui::Text("Ignore");
                        ImGui::Image((void*)(intptr_t)gfx->getGeometryPassIgnoreBuffer(),PreviewSize, UV1, UV2);

                        ImGui::Columns(1);
                    }

                    if(ImGui::CollapsingHeader("Shadow Pass"))
                    {
                        ImGui::Text("Depth");
                        ImGui::Image((void*)(intptr_t)gfx->getShadowPassDepthBuffer(),PreviewSize,UV1,UV2);
                    }
                }
            }
            ImGui::End();
        }
    }

    ImVec2 RenderingDebugWindow::PreviewSize = ImVec2(256,256);
    ImVec2 RenderingDebugWindow::UV1 = ImVec2(0,1);
    ImVec2 RenderingDebugWindow::UV2 = ImVec2(1,0);
}

#include "RenderPipelinePreviewWindow.h"
#include "../../DTState.h"
#include "../../../DreamCore/Project/Project.h"
#include "../../../DreamCore/Project/ProjectRuntime.h"
#include "../../../DreamCore/Components/Graphics/GraphicsComponent.h"

using namespace Dream;

namespace DreamTool
{
    RenderPipelinePreviewWindow::RenderPipelinePreviewWindow
    (DTState* state)
        :ImGuiWidget(state,false)
    {
#ifdef DREAM_LOG
        setLogClassName("RenderPipelinePreviewWidget");
#endif
    }

    RenderPipelinePreviewWindow::~RenderPipelinePreviewWindow
    ()
    {

    }


    void
    RenderPipelinePreviewWindow::draw
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

    ImVec2 RenderPipelinePreviewWindow::PreviewSize = ImVec2(192,192);
    ImVec2 RenderPipelinePreviewWindow::UV1 = ImVec2(0,1);
    ImVec2 RenderPipelinePreviewWindow::UV2 = ImVec2(1,0);
}

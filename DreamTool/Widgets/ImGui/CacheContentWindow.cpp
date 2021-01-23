#include "CacheContentWindow.h"
#include "DreamToolContext.h"
#include <DreamCore.h>

namespace octronic::dream::tool
{
    CacheContentWindow::CacheContentWindow
    (DreamToolContext* proj) : ImGuiWidget(proj,false)
    {
    }

    CacheContentWindow::~CacheContentWindow
    ()
    {

    }

    void
    CacheContentWindow::draw
    ()
    {
        ImGui::Begin("Cache Content");

        if (mContext->getProject())
        {
            auto projRunt = mContext->getProject()->getRuntime();
            if (projRunt)
            {
                if (ImGui::CollapsingHeader("Audio Cache"))
                {
                    auto cache = projRunt->getAudioCache();
                    for (auto instance : cache->getRuntimeVector())
                    {
                        ImGui::Text("%s",instance->getNameAndUuidString().c_str());
                    }
                }

                if (ImGui::CollapsingHeader("Material Cache"))
                {
                    auto cache = projRunt->getMaterialCache();
                    for (auto instance : cache->getRuntimeVector())
                    {
                        ImGui::Text("%s",instance->getNameAndUuidString().c_str());
                    }
                }

                if (ImGui::CollapsingHeader("Model Cache"))
                {
                    auto cache = projRunt->getModelCache();
                    for (auto instance : cache->getRuntimeVector())
                    {
                        ImGui::Text("%s",instance->getNameAndUuidString().c_str());
                    }
                }

                if (ImGui::CollapsingHeader("Script Cache"))
                {
                    auto cache = projRunt->getScriptCache();
                    for (auto instance : cache->getRuntimeVector())
                    {
                        ImGui::Text("%s",instance->getNameAndUuidString().c_str());
                    }
                }

                if (ImGui::CollapsingHeader("Shader Cache"))
                {
                    auto cache = projRunt->getShaderCache();
                    for (auto instance : cache->getRuntimeVector())
                    {
                        ImGui::Text("%s",instance->getNameAndUuidString().c_str());
                    }
                }

                if (ImGui::CollapsingHeader("Texture Cache"))
                {
                    auto cache = projRunt->getTextureCache();
                    for (auto instance : cache->getRuntimeVector())
                    {
                        ImGui::Text("%s",instance->getNameAndUuidString().c_str());
                    }
                }

            }

        }
        ImGui::End();
    }
}

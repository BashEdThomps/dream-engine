#include "CacheContentWindow.h"
#include "../../DTState.h"
#include "../../../DreamCore/Project/Project.h"
#include "../../../DreamCore/Project/ProjectRuntime.h"
#include "../../../DreamCore/Components/Audio/AudioCache.h"
#include "../../../DreamCore/Components/Graphics/Material/MaterialCache.h"
#include "../../../DreamCore/Components/Graphics/Model/ModelCache.h"
#include "../../../DreamCore/Components/Graphics/Shader/ShaderCache.h"
#include "../../../DreamCore/Components/Graphics/Texture/TextureCache.h"
#include "../../../DreamCore/Components/Scripting/ScriptCache.h"

namespace DreamTool
{
    CacheContentWindow::CacheContentWindow
    (DTState* proj) : ImGuiWidget(proj,false)
    {
#ifdef DREAM_LOG
        setLogClassName("CacheContentWindow");
#endif
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

        if (mState->project)
        {
            auto projRunt = mState->project->getRuntime();
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

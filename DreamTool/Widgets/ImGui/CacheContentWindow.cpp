#include "CacheContentWindow.h"
#include "DreamToolContext.h"
#include <DreamCore.h>

namespace octronic::dream::tool
{
  CacheContentWindow::CacheContentWindow
  (DreamToolContext& proj) : ImGuiWidget(proj,false)
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

    auto& ctx = getContext();
    auto& pCtxOpt = ctx.getProjectContext();


    if (pCtxOpt)
    {
      auto& pCtx = pCtxOpt.value();

      auto& pRuntOpt = pCtx.getProjectRuntime();

      if (pRuntOpt)
      {
        auto& pRunt = pRuntOpt.value();

        char title[128] = {0};
        string formatStr = "%s Cache (%d)";

        auto& audioCache = pRunt.getAudioCache();
        snprintf(title, 128, formatStr.c_str(), "Audio", audioCache.runtimeCount());
        if (ImGui::CollapsingHeader(title))
        {
          for (auto instance : audioCache.getRuntimeVector())
          {
            ImGui::Text("%s",instance.get().getNameAndUuidString().c_str());
          }
        }

        auto& fontCache = pRunt.getFontCache();
        snprintf(title, 128, formatStr.c_str(), "Font", fontCache.runtimeCount());

        if (ImGui::CollapsingHeader(title))
        {
          for (auto& instance : fontCache.getRuntimeVector())
          {
            ImGui::Text("%s",instance.get().getNameAndUuidString().c_str());
          }
        }

        auto& materialCache = pRunt.getMaterialCache();
        snprintf(title, 128, formatStr.c_str(), "Material", materialCache.runtimeCount());
        if (ImGui::CollapsingHeader(title))
        {
          for (auto& instance : materialCache.getRuntimeVector())
          {
            ImGui::Text("%s",instance.get().getNameAndUuidString().c_str());
          }
        }

        auto& modelCache = pRunt.getModelCache();
        snprintf(title, 128, formatStr.c_str(), "Model", modelCache.runtimeCount());
        if (ImGui::CollapsingHeader(title))
        {
          for (auto& instance : modelCache.getRuntimeVector())
          {
            ImGui::Text("%s",instance.get().getNameAndUuidString().c_str());
          }
        }

        auto& scriptCache = pRunt.getScriptCache();
        snprintf(title, 128, formatStr.c_str(), "Script", scriptCache.runtimeCount());
        if (ImGui::CollapsingHeader(title))
        {
          for (auto& instance : scriptCache.getRuntimeVector())
          {
            ImGui::Text("%s",instance.get().getNameAndUuidString().c_str());
          }
        }

        auto& shaderCache = pRunt.getShaderCache();
        snprintf(title, 128, formatStr.c_str(), "Shader", shaderCache.runtimeCount());
        if (ImGui::CollapsingHeader(title))
        {
          for (auto& instance : shaderCache.getRuntimeVector())
          {
            ImGui::Text("%s",instance.get().getNameAndUuidString().c_str());
          }
        }

        auto& textureCache = pRunt.getTextureCache();
        snprintf(title, 128, formatStr.c_str(), "Texture", textureCache.runtimeCount());
        if (ImGui::CollapsingHeader(title))
        {
          for (auto& instance : textureCache.getRuntimeVector())
          {
            ImGui::Text("%s",instance.get().getNameAndUuidString().c_str());
          }
        }
      }
    }
    ImGui::End();
  }
}

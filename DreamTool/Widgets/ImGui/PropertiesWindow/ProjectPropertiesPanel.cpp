#include "ProjectPropertiesPanel.h"

#include "PropertiesWindow.h"
#include "DreamToolContext.h"

using std::exception;

namespace octronic::dream::tool
{
  ProjectPropertiesPanel::ProjectPropertiesPanel
  (ImGuiWidget& parent)
    : ImGuiPanel(parent),
      mNameAndUuidPanel(parent)
  {

  }

  void
  ProjectPropertiesPanel::draw
  ()
  {
    auto& parent = static_cast<PropertiesWindow&>(getParent());
    auto& dtCtx = parent.getContext();
    auto& pCtxOpt = dtCtx.getProjectContext();

    if (pCtxOpt)
    {
      auto target = parent.getCurrentPropertyTarget();
      auto pDefOpt = target.getDefinition();
      auto& projectDefinition = static_cast<ProjectDefinition&>(pDefOpt.value().get());

      ImGui::SameLine();

      if(ImGui::Button("New Scene"))
      {
        auto& newScene = projectDefinition.createNewSceneDefinition();
        parent.pushPropertyTarget({PropertyType_Scene, newScene});
        return;
      }

      mNameAndUuidPanel.draw();

      ImGui::Separator();
      // Startup Scene
      int startupSceneIndex = -1;
      auto startupOpt = projectDefinition.getStartupSceneDefinition();
      if(startupOpt)
      {
			  auto& startup = startupOpt.value().get();
			  startupSceneIndex = projectDefinition.getSceneDefinitionIndex(startup);
      }

      vector<string> scenes;
      for(auto& scene : projectDefinition.getSceneDefinitionsVector())
      {
        scenes.push_back(scene.getName());
      }
      if(ImGuiWidget::StringCombo("Startup Scene",&startupSceneIndex,scenes,scenes.size()))
      {
        auto& newStartup = projectDefinition.getSceneDefinitionAtIndex(startupSceneIndex);
        projectDefinition.setStartupSceneUuid(newStartup.getUuid());
      }
      ImGui::Separator();
    }
  }
}

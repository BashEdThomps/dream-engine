#include "IdentityPropertiesPanel.h"

#include "PropertiesWindow.h"

namespace octronic::dream::tool
{
  IdentityPropertiesPanel::IdentityPropertiesPanel
  (ImGuiWidget& parent)
    : ImGuiPanel(parent)
  {

  }

  void
  IdentityPropertiesPanel::draw
  ()
  {
    ImGui::Separator();

    ImGui::Text("Identity");

    auto& parent = static_cast<PropertiesWindow&>(getParent());
    if (parent.getCurrentPropertyTarget().has_value())
    {
      auto currentTarget = parent.getCurrentPropertyTarget().value().get();
      auto currentDef = currentTarget.getDefinition();

      if (currentDef)
      {
        auto& def = currentDef.value().get();

        // UUID

        int id = def.getUuid();
        ImGui::InputInt("ID", &id);


        char textBuf[128] = {0};

        // Name

        memset(textBuf,0,128);
        strncpy(&textBuf[0], def.getName().c_str(), def.getName().size());

        if(ImGui::InputText("Name", textBuf, IM_ARRAYSIZE(textBuf)))
        {
          def.setName(textBuf);
        }

        // Group

        memset(textBuf,0,128);
        strncpy(&textBuf[0], def.getGroup().c_str(), def.getGroup().size());

        if(ImGui::InputText("Group", textBuf, IM_ARRAYSIZE(textBuf)))
        {
          def.setGroup(textBuf);
        }
      }
    }
  }
}

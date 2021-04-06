#include "NameAndUuidPanel.h"

#include "PropertiesWindow.h"

namespace octronic::dream::tool
{
  NameAndUuidPanel::NameAndUuidPanel
  (ImGuiWidget& parent)
    : ImGuiPanel(parent)
  {

  }

  void
  NameAndUuidPanel::draw
  ()
  {
    auto& parent = static_cast<PropertiesWindow&>(getParent());
    auto currentTarget = parent.getCurrentPropertyTarget();
    auto currentDef = currentTarget.getDefinition();

    if (currentDef)
    {
      auto& def = currentDef.value().get();
      char nameBuf[128] = {0};
      strncpy(&nameBuf[0], def.getName().c_str(), def.getName().size());
      if(ImGui::InputText("Name", nameBuf, IM_ARRAYSIZE(nameBuf)))
      {
        def.setName(nameBuf);
      }

      int id = def.getUuid();
      if(ImGui::InputInt("ID", &id))
      {
        def.setUuid(id);
      }
    }
  }
}

#pragma once

#include "Widgets/ImGui/ImGuiPanel.h"
#include "IdentityPropertiesPanel.h"

namespace octronic::dream::tool
{
  class ProjectPropertiesPanel : public ImGuiPanel
  {
  public:
    ProjectPropertiesPanel(ImGuiWidget& parent);
    void draw() override;
  private:
    IdentityPropertiesPanel mNameAndUuidPanel;
  };
}


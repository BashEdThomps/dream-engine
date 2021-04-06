#pragma once

#include "Widgets/ImGui/ImGuiPanel.h"
#include "NameAndUuidPanel.h"

namespace octronic::dream::tool
{
  class ProjectPropertiesPanel : public ImGuiPanel
  {
  public:
    ProjectPropertiesPanel(ImGuiWidget& parent);
    void draw() override;
  private:
    NameAndUuidPanel mNameAndUuidPanel;
  };
}


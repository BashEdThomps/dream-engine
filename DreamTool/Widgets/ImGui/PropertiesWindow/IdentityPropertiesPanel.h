#pragma once

#include "Widgets/ImGui/ImGuiPanel.h"

namespace octronic::dream::tool
{
  class IdentityPropertiesPanel : public ImGuiPanel
  {
  public:
    IdentityPropertiesPanel(ImGuiWidget& parent);
    void draw() override;
  };
}

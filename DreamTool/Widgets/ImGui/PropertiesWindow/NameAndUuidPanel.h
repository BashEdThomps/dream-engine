#pragma once

#include "Widgets/ImGui/ImGuiPanel.h"

namespace octronic::dream::tool
{
  class NameAndUuidPanel : public ImGuiPanel
  {
  public:
    NameAndUuidPanel(ImGuiWidget& parent);
    void draw() override;
  };
}

#pragma once

#include "Widgets/ImGui/ImGuiPanel.h"

#include <DreamCore.h>

using octronic::dream::Transform;

namespace octronic::dream::tool
{
  class TransformPropertiesPanel : public ImGuiPanel
  {
  public:
    TransformPropertiesPanel(ImGuiWidget& parent);
    void setTransform(Transform&);
    void draw() override;
  private:
    optional<reference_wrapper<Transform>> mTransform;
  };
}


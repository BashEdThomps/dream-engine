#pragma once

#include "ImGuiWidget.h"

namespace octronic::dream
{
  class TemplateEntityDefinition;
  class Definition;
}

#define buf_sz 128

namespace octronic::dream::tool
{
  class DefinitionBrowser : public ImGuiWidget
  {
  public:
    DefinitionBrowser(DreamToolContext& proj);
    void draw() override;

  private:
    const ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
    const ImGuiTreeNodeFlags leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
    vector<reference_wrapper<Definition>> mSelectedNodes;
    map<string, bool> mOpenHeaders;
  };
}

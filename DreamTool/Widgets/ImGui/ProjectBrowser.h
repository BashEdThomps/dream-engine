#pragma once

#include "ImGuiWidget.h"

using std::optional;
using std::reference_wrapper;

namespace octronic::dream
{
  class TemplateEntityDefinition;
  class Definition;
}

#define buf_sz 128

namespace octronic::dream::tool
{
  struct SceneEntityDragDropSource
  {
    optional<reference_wrapper<SceneEntityDefinition>> parentDefinition;
    optional<reference_wrapper<SceneEntityDefinition>> selfDefinition;
  };

  class ProjectBrowser : public ImGuiWidget
  {
  public:
    ProjectBrowser(DreamToolContext& proj);
    void draw() override;

    void removeNodeSelection(Definition& def);

  private:
    void addEntityNode(SceneEntityDefinition& def);
    const ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
    const ImGuiTreeNodeFlags leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
    vector<reference_wrapper<Definition>> mSelectedNodes;
    SceneEntityDragDropSource mDragDropSource;
  };
}

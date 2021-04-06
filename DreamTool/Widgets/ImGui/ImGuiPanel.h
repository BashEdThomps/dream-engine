#pragma once

#include <memory>

using std::reference_wrapper;

namespace octronic::dream::tool
{
  class ImGuiWidget;

  class ImGuiPanel
  {
  public:
    ImGuiPanel(ImGuiWidget& parent);
    virtual ~ImGuiPanel();
    virtual void draw() = 0;
    ImGuiWidget& getParent();
  private:
    reference_wrapper<ImGuiWidget> mParent;
  };
}

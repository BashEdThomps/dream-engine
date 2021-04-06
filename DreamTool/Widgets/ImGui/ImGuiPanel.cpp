#include "ImGuiPanel.h"

namespace octronic::dream::tool
{
	ImGuiPanel::ImGuiPanel
  (ImGuiWidget& parent)
    : mParent(parent)
	{

	}

  ImGuiPanel::~ImGuiPanel
  ()
  {

  }

  ImGuiWidget&
  ImGuiPanel::getParent
  ()
  {
    return mParent.get();
  }
}

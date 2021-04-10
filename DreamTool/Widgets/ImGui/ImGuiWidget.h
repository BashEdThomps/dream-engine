#pragma once

#include "Widgets/DreamToolWidget.h"
#include <imgui.h>

namespace octronic::dream::tool
{
  class ImGuiWidget : public DreamToolWidget
  {
  public:
    ImGuiWidget(DreamToolContext& project, bool visible);
    static bool StringCombo(const char* label, int* current_item,const vector<string>& items);
    static int GetStringIndexInVector(const string& str, const vector<string>& vec);
  };
}

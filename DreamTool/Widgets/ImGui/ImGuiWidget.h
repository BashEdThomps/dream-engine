//
//  ImGuiWidget.h
//  DreamToolImGui
//
//  Created by Ashley Thompson on 24/10/2018.
//

#pragma once

#include "Widgets/DreamToolWidget.h"
#include <imgui.h>

namespace octronic::dream::tool
{
  class ImGuiWidget : public DreamToolWidget
  {
  public:
    ImGuiWidget(DreamToolContext& project,bool visible = true);
    ~ImGuiWidget() override;
    static bool StringCombo(const char* label, int* current_item,const vector<string>& items,int height_in_items = -1);
    static int GetStringIndexInVector(const string& str, const vector<string>& vec);
  };
}

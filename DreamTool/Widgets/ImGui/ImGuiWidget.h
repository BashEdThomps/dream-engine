//
//  ImGuiWidget.h
//  DreamToolImGui
//
//  Created by Ashley Thompson on 24/10/2018.
//

#pragma once

#include "../DTWidget.h"
#include "../../deps/ImGui/imgui.h"

namespace DreamTool
{
    class ImGuiWidget : public DTWidget
    {
    public:
        ImGuiWidget(Project* project);
        ~ImGuiWidget() override;
        bool StringCombo(const char* label, int* current_item, const vector<string>& items, int items_count, int height_in_items = -1);
    };
}

//
//  ImGuiWidget.cpp
//  DreamToolImGui
//
//  Created by Ashley Thompson on 24/10/2018.
//

#include "ImGuiWidget.h"

namespace DreamTool
{
    ImGuiWidget::ImGuiWidget
    (DTContext* project, bool visible)
        : DTWidget(project, visible)
    {
    }

    ImGuiWidget::~ImGuiWidget() {}

    bool
    ImGuiWidget::StringCombo
    (
        const char* label, int* current_item,
        const vector<string>& items,
        size_t items_count, int height_in_items
    )
    {
        return ImGui::Combo
        (
            label,
            current_item,
            [](void* data, int idx, const char** out_text)
            {
                auto _data = ((const vector<string>*)data);
                *out_text = (*_data)[idx].c_str();
                return true;
            },
            (void*)&items,
            items_count,
            height_in_items
            );
    }
}

#pragma once

#include "ImGuiWidget.h"

namespace DreamTool
{
    class Grid;
    class ToolPropertiesWindow : public ImGuiWidget
    {
    public:
        ToolPropertiesWindow(DTState* p);
        ~ToolPropertiesWindow() override;
        void draw() override;
    };
}

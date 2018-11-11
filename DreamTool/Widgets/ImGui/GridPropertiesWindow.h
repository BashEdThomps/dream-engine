#pragma once

#include "ImGuiWidget.h"

namespace DreamTool
{
    class Grid;
    class GridPropertiesWindow : public ImGuiWidget
    {
    public:
        GridPropertiesWindow(DTState* p);
        ~GridPropertiesWindow() override;
        void draw() override;
    };
}

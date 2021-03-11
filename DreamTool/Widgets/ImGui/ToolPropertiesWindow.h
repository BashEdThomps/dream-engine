#pragma once

#include "ImGuiWidget.h"

namespace octronic::dream::tool
{
    class Grid;
    class ToolPropertiesWindow : public ImGuiWidget
    {
    public:
        ToolPropertiesWindow(DreamToolContext* p);
        ~ToolPropertiesWindow() override;
        void draw() override;
    protected:
        void drawCursorSettings();
        void drawSelectionSettings();
        void drawGridSettings();
    	void drawLightIndicatorSettings();
    };
}

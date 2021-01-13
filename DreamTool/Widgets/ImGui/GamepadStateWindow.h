#pragma once

#include "ImGuiWidget.h"

namespace DreamTool
{
    class GamepadStateWindow : public ImGuiWidget
    {
    public:
        GamepadStateWindow(DTContext*, bool visible = false);
        ~GamepadStateWindow() override;
        void draw() override;
    };
}

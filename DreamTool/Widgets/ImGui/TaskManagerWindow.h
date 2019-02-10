#pragma once

#include "ImGuiWidget.h"

using namespace Dream;

namespace DreamTool
{
    class TaskManagerWindow : public ImGuiWidget
    {
    public:
        TaskManagerWindow(DTState* proj);
        ~TaskManagerWindow() override;
        void draw() override;
    };
}

#pragma once

#include "ImGuiWidget.h"


namespace octronic::dream::tool
{
    class TaskManagerWindow : public ImGuiWidget
    {
    public:
        TaskManagerWindow(DreamToolContext& proj);
        ~TaskManagerWindow() override;
        void draw() override;
    };
}

#pragma once

#include "ImGuiWidget.h"

namespace DreamTool
{
    class SceneStateWindow
        : public ImGuiWidget
    {
    public:
        SceneStateWindow(DTContext*);
        ~SceneStateWindow() override;

        void draw() override;
    };
}

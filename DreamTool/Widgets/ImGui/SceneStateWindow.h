#pragma once

#include "ImGuiWidget.h"

namespace DreamTool
{
    class SceneStateWindow
        : public ImGuiWidget
    {
    public:
        SceneStateWindow(DTState*);
        ~SceneStateWindow() override;

        void draw() override;
    };
}

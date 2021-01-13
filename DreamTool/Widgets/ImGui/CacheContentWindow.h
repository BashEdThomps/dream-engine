#pragma once

#include "ImGuiWidget.h"

using namespace Dream;

namespace DreamTool
{
    class CacheContentWindow : public ImGuiWidget
    {
    public:
        CacheContentWindow(DTContext* proj);
        ~CacheContentWindow() override;
        void draw() override;
    };
}

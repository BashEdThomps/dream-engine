#pragma once

#include "ImGuiWidget.h"

namespace octronic::dream::tool
{
    class CacheContentWindow : public ImGuiWidget
    {
    public:
        CacheContentWindow(DreamToolContext* proj);
        ~CacheContentWindow() override;
        void draw() override;
    };
}

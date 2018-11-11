#pragma once

#include "ImGuiWidget.h"

namespace DreamTool
{
    class LuaDebugWindow
            : public ImGuiWidget,
              public LuaPrintListener
    {
    public:
        LuaDebugWindow(DTState* proj);
        ~LuaDebugWindow() override;

        void draw() override;
        void onPrint(string) override;
    private:
        ImGuiTextBuffer mLogBuffer;
    };
}

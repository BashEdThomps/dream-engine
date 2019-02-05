#pragma once

#include "ImGuiWidget.h"
#include "../../../DreamCore/Components/Script/ScriptComponent.h"

using namespace Dream;

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
        void onPrint(const string&) override;
    private:
        ImGuiTextBuffer mLogBuffer;
    };
}

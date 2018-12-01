#pragma once

#include "ImGuiWidget.h"
#include "../../../DreamCore/Components/Scripting/ScriptComponent.h"

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
        void onPrint(string) override;
    private:
        ImGuiTextBuffer mLogBuffer;
    };
}

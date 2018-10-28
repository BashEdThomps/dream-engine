#pragma once

#include "DTWidget.h"

using Dream::LuaPrintListener;

namespace DreamTool
{
    class LuaDebugWindow
            : public DTWidget,
              public LuaPrintListener
    {
    public:
        LuaDebugWindow(Dream::Project* proj);
        ~LuaDebugWindow() override;

        void draw() override;
        void onPrint(string) override;
    };
}

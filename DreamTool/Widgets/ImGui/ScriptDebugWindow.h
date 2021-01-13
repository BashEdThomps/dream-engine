#pragma once

#include "ImGuiWidget.h"
#include <DreamCore.h>

using namespace Dream;

namespace DreamTool
{
    class ScriptDebugWindow
            : public ImGuiWidget,
              public ScriptPrintListener
    {
    public:
        ScriptDebugWindow(DTContext* proj);
        ~ScriptDebugWindow() override;

        void draw() override;
        void onPrint(const string&) override;
    private:
        ImGuiTextBuffer mLogBuffer;
    };
}

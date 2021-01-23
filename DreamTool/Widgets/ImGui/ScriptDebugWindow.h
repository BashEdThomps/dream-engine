#pragma once

#include "ImGuiWidget.h"
#include <DreamCore.h>

using octronic::dream::ScriptPrintListener;

namespace octronic::dream::tool
{
    class ScriptDebugWindow : public ImGuiWidget, public ScriptPrintListener
    {
    public:
        ScriptDebugWindow(DreamToolContext* proj);
        ~ScriptDebugWindow() override;

        void draw() override;
        void onPrint(const string&) override;
    private:
        ImGuiTextBuffer mLogBuffer;
    };
}

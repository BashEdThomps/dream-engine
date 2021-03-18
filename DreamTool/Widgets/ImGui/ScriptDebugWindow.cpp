#include "ScriptDebugWindow.h"
#include "DreamToolContext.h"

namespace octronic::dream::tool
{
    ScriptDebugWindow::ScriptDebugWindow
    (DreamToolContext* proj)
        : ImGuiWidget(proj,false),
        ScriptPrintListener()
    {
    }

    ScriptDebugWindow::~ScriptDebugWindow
    ()
    {

    }

    void
    ScriptDebugWindow::draw
    ()
    {
        auto project = mContext->getProject();
        if (project)
        {
            ImGui::Begin("Script Debug Output",&mVisible);

            if(ImGui::Button("Clear"))
            {
                mLogBuffer.clear();
            }

            ImGui::SameLine();

            static bool scrollToBottom = false;
            ImGui::Checkbox("AutoScroll",&scrollToBottom);

            ImGui::Separator();
            ImGui::BeginChild("scrolling");
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,1));

            ImGui::TextUnformatted(mLogBuffer.begin());

            if (scrollToBottom)
            {
                ImGui::SetScrollHere(1.0f);
            }

            ImGui::PopStyleVar();
            ImGui::EndChild();

            ImGui::End();
        }
    }

    void
    ScriptDebugWindow::onPrint
    (const string& msg)
    {
        mLogBuffer.appendf("%s\n",msg.c_str());
    }
}

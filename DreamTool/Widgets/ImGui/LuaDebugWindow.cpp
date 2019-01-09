#include "LuaDebugWindow.h"
#include "../../DTState.h"

namespace DreamTool
{
    LuaDebugWindow::LuaDebugWindow
    (DTState* proj)
        : ImGuiWidget(proj,false),
        LuaPrintListener()
    {
#ifdef DREAM_LOG
        setLogClassName("LuaDebugWindow");
#endif
    }

    LuaDebugWindow::~LuaDebugWindow
    ()
    {

    }

    void
    LuaDebugWindow::draw
    ()
    {
        if (mState->project)
        {
            ImGui::Begin("Lua Debug Output",&mVisible);

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
    LuaDebugWindow::onPrint
    (const string& msg)
    {
        mLogBuffer.appendf("%s\n",msg.c_str());
    }
}

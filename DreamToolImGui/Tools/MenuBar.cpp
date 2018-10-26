#include "MenuBar.h"

namespace DreamTool
{
    MenuBar::MenuBar
    (Project* def)
        : DTWidget(def)
    {

    }

    MenuBar::~MenuBar
    ()
    {

    }
    void
    MenuBar::draw
    ()
    {
        static bool someBool = false;
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("New");
                ImGui::MenuItem("Open");
                ImGui::MenuItem("Save");
                ImGui::MenuItem("Quit");
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Scene"))
            {
                static int index = 0;
                char* scenes[3] = {"Scene 1","Scene 2","Scene 3"};
                ImGui::MenuItem("Start Scene");
                ImGui::MenuItem("Stop Scene");
                ImGui::Separator();
                ImGui::Combo("##hidelabel",&index,scenes[0],3);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Debug"))
            {
                ImGui::Checkbox("Physics Debug",&someBool);
                ImGui::Checkbox("Lua Debug",&someBool);
                if(ImGui::BeginMenu("Logging"))
                {
                    ImGui::MenuItem("Off");
                    ImGui::MenuItem("Critical");
                    ImGui::MenuItem("Error");
                    ImGui::MenuItem("Warning");
                    ImGui::MenuItem("Info");
                    ImGui::MenuItem("Debug");
                    ImGui::MenuItem("Trace");
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
}

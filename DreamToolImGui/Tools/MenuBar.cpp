#include "MenuBar.h"
#include "ImGuiHelpers.h"
using Dream::SceneDefinition;

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
				

				if (ImGui::MenuItem("Start Scene"))
				{

				}
				if (ImGui::MenuItem("Stop Scene"))
				{

				}

                ImGui::Separator();

				auto scenesVector = mProject->getProjectDefinition()->getSceneDefinitionsVector();
				vector<string> sceneNames;
				for (auto scene : scenesVector)
				{
					sceneNames.push_back(scene->getName());
				}

				auto currentScene = dynamic_cast<SceneDefinition*>(mProject->getProjectRuntime()->getActiveSceneRuntime()->getDefinition());
                static int startupSceneIndex = mProject->getProjectDefinition()->getSceneDefinitionIndex(currentScene);
				if (StringCombo("Active Scene", &startupSceneIndex, sceneNames, sceneNames.size()))
				{
					
				}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Debug"))
            {
                ImGui::MenuItem("Lua Debug Window");
                if(ImGui::BeginMenu("Logging"))
                {
                    static spdlog::level::level_enum mode = spdlog::level::off;
                    if (ImGui::RadioButton("Off", mode == spdlog::level::off))
                    {
                        mode = spdlog::level::off;
                        spdlog::set_level(mode);
                    }
                    if (ImGui::RadioButton("Error", mode == spdlog::level::err))
                    {
                        mode = spdlog::level::err;
                        spdlog::set_level(mode);
                    }
                    if (ImGui::RadioButton("Critical", mode == spdlog::level::critical))
                    {
                        mode = spdlog::level::critical;
                        spdlog::set_level(mode);
                    }
                    if (ImGui::RadioButton("Warning", mode == spdlog::level::warn))
                    {
                        mode = spdlog::level::warn;
                        spdlog::set_level(mode);
                    }
                    if (ImGui::RadioButton("Info", mode == spdlog::level::info))
                    {
                        mode = spdlog::level::info;
                        spdlog::set_level(mode);
                    }
                    if (ImGui::RadioButton("Debug", mode == spdlog::level::debug))
                    {
                        mode = spdlog::level::debug;
                        spdlog::set_level(mode);
                    }
                    if (ImGui::RadioButton("Trace", mode == spdlog::level::trace))
                    {
                        mode = spdlog::level::trace;
                        spdlog::set_level(mode);
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
}

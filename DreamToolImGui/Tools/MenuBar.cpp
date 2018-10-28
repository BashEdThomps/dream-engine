#include "MenuBar.h"
#include "ImGuiHelpers.h"
#include "../deps/ImGui/imgui_internal.h"

using Dream::SceneDefinition;
using Dream::SceneState;
using Dream::SceneRuntime;

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

            if (ImGui::BeginMenu("View"))
            {
                ImGui::DragFloat("Text Scaling", &(ImGui::GetCurrentContext()->Font->Scale),0.1f,1.0f,10.0f);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Scene"))
            {

                SceneRuntime* sceneRuntime = nullptr;// TODO mProject->getProjectRuntime()->getActiveSceneRuntime();
                if (ImGui::MenuItem("Start Scene"))
                {
                    if(sceneRuntime)
                    {
                        sceneRuntime->setState(SceneState::SCENE_STATE_NOT_LOADED);
                    }
                }
                if (ImGui::MenuItem("Stop Scene"))
                {
                    if(sceneRuntime)
                    {
                        sceneRuntime->setState(SceneState::SCENE_STATE_STOPPED);
                    }
                }

                ImGui::Separator();

                auto scenesVector = mProject->getProjectDefinition()->getSceneDefinitionsVector();
                vector<string> sceneNames;
                for (auto scene : scenesVector)
                {
                    sceneNames.push_back(scene->getName());
                }

                SceneDefinition* currentScene = nullptr;
                // TODO dynamic_cast<SceneDefinition*>(mProject->getProjectRuntime()->getActiveSceneRuntime()->getDefinition());
                static int startupSceneIndex = mProject->getProjectDefinition()->getSceneDefinitionIndex(currentScene);
                if (StringCombo("Active Scene", &startupSceneIndex, sceneNames, sceneNames.size()))
                {

                }

                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu("Components"))
            {
                if (ImGui::BeginMenu("Camera"))
                {
                    static int mode = 1;
                    if (ImGui::RadioButton("Free Mode",mode == 0))
                    {

                    }

                    if (ImGui::RadioButton("Look At", mode == 1))
                    {

                    }

                    if (ImGui::RadioButton("Scripted", mode == 2))
                    {

                    }

                    ImGui::EndMenu();
                }
                float volume = 1.0f;
                if(ImGui::SliderFloat("Volume",&volume,0.0f,1.0f))
                {

                }
                auto pRuntime = mProject->getProjectRuntime();
                bool scripting = pRuntime->getScriptingEnabled();
                if(ImGui::Checkbox("Scripting",&scripting))
                {
                  pRuntime->setScriptingEnabled(scripting);
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

#include "MenuBar.h"
#include "ImGuiHelpers.h"
#include "../deps/ImGui/imgui_internal.h"

using Dream::SceneDefinition;
using Dream::SceneState;
using Dream::SceneRuntime;
using Dream::ProjectRuntime;

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
        bool showQuit = false;
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if(ImGui::MenuItem("Quit"))
                {
                    showQuit = true;
                }
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

                vector<string> sceneNames;
                static int sceneIndex = -1;

                if (mProject)
                {
                    auto projDef = mProject->getProjectDefinition();
                    if (projDef)
                    {
                        auto scenesVector = projDef->getSceneDefinitionsVector();
                        for (auto scene : scenesVector)
                        {
                            sceneNames.push_back(scene->getName());
                        }
                    }
                }

                if (StringCombo("Active Scene", &sceneIndex, sceneNames, sceneNames.size()))
                {
                    ImGui::OpenPopup("Change Scene?");
                }

                if (ImGui::BeginPopupModal("Change Scene?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    ImGui::Text("Do you want to switch to this Scene?");
                    ImGui::Separator();

                    if (ImGui::Button("Not Now", ImVec2(120, 0)))
                    {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Switch Scene", ImVec2(120, 0)))
                    {
                        if (mProject)
                        {
                            auto projDef = mProject->getProjectDefinition();
                            if (projDef)
                            {
                                auto selectedSceneDef = projDef->getSceneDefinitionAtIndex(sceneIndex);
                                auto projRunt = mProject->getProjectRuntime();
                                if (projRunt)
                                {
                                    if (ProjectRuntime::CurrentSceneRuntime)
                                    {
                                        ProjectRuntime::CurrentSceneRuntime->setState(SceneState::SCENE_STATE_STOPPED);
                                        projRunt->destructSceneRuntime(ProjectRuntime::CurrentSceneRuntime);
                                        ProjectRuntime::CurrentSceneRuntime = nullptr;
                                    }
                                    ProjectRuntime::CurrentSceneRuntime = projRunt->constructSceneRuntime(selectedSceneDef);
                                }
                            }
                        }
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::SetItemDefaultFocus();
                    ImGui::EndPopup();
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
                bool scripting = false;
                if (pRuntime)
                {
                    scripting = pRuntime->getScriptingEnabled();
                }
                if(ImGui::Checkbox("Scripting",&scripting))
                {
                    if(pRuntime)
                    {
                        pRuntime->setScriptingEnabled(scripting);
                    }
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

        if (showQuit)
        {
            ImGui::OpenPopup("Quit?");
        }

        if (ImGui::BeginPopupModal("Quit?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Are you sure you want to quit?\n\nAny unsaved changes will be lost.\n\n");
            ImGui::Separator();

            if (ImGui::Button("Cancel##cancelQuit", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Quit##confirmQuit", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }
    }
}

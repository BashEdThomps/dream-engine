#include "MenuBar.h"
#include "LuaDebugWindow.h"
#include "ProjectBrowser.h"
#include "PropertiesWindow.h"
#include "SceneStateWindow.h"
#include "../../DTState.h"
#include "../../deps/ImGui/imgui_internal.h"
#include "../../deps/ImGui/imguifilesystem.h"

namespace DreamTool
{
    MenuBar::MenuBar(DTState* def)
        : ImGuiWidget(def)
    {
        setLogClassName("MenuBar");
    }

    MenuBar::~MenuBar
    ()
    {

    }
    void
    MenuBar::draw
    ()
    {
        auto log = getLog();

        bool showQuitDialog = false;
        bool newButtonClicked = false;
        bool openButtonClicked = false;
        bool showSaveSuccessDialog = false;
        bool showPleaseDestroyScenesDialog = false;

        auto pRuntime = mState->project->getProjectRuntime();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                newButtonClicked = ImGui::MenuItem("New");
                openButtonClicked = ImGui::MenuItem("Open");
                if(ImGui::MenuItem("Save"))
                {
                    ProjectDirectory pDir(mState->project);
                    if(pDir.saveProject())
                    {
                        showSaveSuccessDialog = true;
                    }
                }
                ImGui::Separator();
                if(ImGui::MenuItem("Close"))
                {

                }
                ImGui::Separator();
                showQuitDialog = ImGui::MenuItem("Quit");
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                bool showProjectBrowser = !mState->projectBrowser.getHidden();
                if(ImGui::Checkbox("Project Browser",&showProjectBrowser))
                {
                    mState->projectBrowser.setHidden(!showProjectBrowser);
                }

                bool showPropertiesWindow = !mState->propertiesWindow.getHidden();
                if(ImGui::Checkbox("Properties Window",&showPropertiesWindow))
                {
                   mState->propertiesWindow.setHidden(!showPropertiesWindow);
                }

                bool showSceneStatesWindow = !mState->sceneStateWindow.getHidden();
                if (ImGui::Checkbox("Scene States",&showSceneStatesWindow))
                {
                   mState->sceneStateWindow.setHidden(!showSceneStatesWindow);
                }

                bool showGridPropsWindow = !mState->gridPropertiesWindow.getHidden();
                if (ImGui::Checkbox("Grid Properties",&showGridPropsWindow))
                {
                   mState->gridPropertiesWindow.setHidden(!showGridPropsWindow);
                }

                ImGui::DragFloat("Text Scaling", &(ImGui::GetCurrentContext()->Font->Scale),0.1f,1.0f,10.0f);

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Scene"))
            {
                vector<string> sceneNames;

                ProjectDefinition* projDef = nullptr;
                if (mState->project)
                {
                    projDef = mState->project->getProjectDefinition();
                    if (projDef)
                    {
                        auto scenesVector = projDef->getSceneDefinitionsVector();
                        for (auto scene : scenesVector)
                        {
                            sceneNames.push_back(scene->getName());
                        }
                    }
                }

                int sceneToLoadIndex = -1;
                if (StringCombo("Set Scene \"To Load\"", &sceneToLoadIndex, sceneNames, sceneNames.size()))
                {
                    auto selectedSceneDef = projDef->getSceneDefinitionAtIndex(sceneToLoadIndex);
                    if (!pRuntime->hasSceneRuntime(selectedSceneDef->getUuid()))
                    {
                        auto newSceneRT = new SceneRuntime(selectedSceneDef,pRuntime);
                        newSceneRT->setState(SceneState::SCENE_STATE_TO_LOAD);
                        pRuntime->addSceneRuntime(newSceneRT);
                    }
                }

                vector<string> runtimeNames;
                if (pRuntime)
                {
                    for (auto s : pRuntime->getSceneRuntimeVector())
                    {
                        // Show only loaded scenes
                        runtimeNames.push_back(s->getName());
                    }
                }

                int sceneActiveIndex = -1;
                if (StringCombo("Set Scene \"Active\"", &sceneActiveIndex, runtimeNames, runtimeNames.size()))
                {
                    if (pRuntime)
                    {
                        auto rt = pRuntime->getSceneRuntimeVector().at(sceneActiveIndex);
                        if (rt)
                        {
                            pRuntime->setSceneRuntimeActive(rt->getUuid());
                        }
                    }
                }

                int sceneToDestroyIndex = -1;
                if (StringCombo("Set Scene \"To Destroy\"", &sceneToDestroyIndex, runtimeNames, runtimeNames.size()))
                {
                    mState->propertiesWindow.clearPropertyTargets();
                    if (pRuntime)
                    {
                        auto rt = pRuntime->getSceneRuntimeVector().at(sceneToDestroyIndex);
                        if (rt)
                        {
                            rt->setState(SceneState::SCENE_STATE_TO_DESTROY);
                        }
                        mState->selectionHighlighter.clearSelection();
                    }
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

                if (ImGui::MenuItem("Clear Caches"))
                {
                    if (pRuntime && !pRuntime->hasLoadedScenes())
                    {
                        pRuntime->clearAllCaches();
                    }
                    else
                    {
                        showPleaseDestroyScenesDialog = true;
                    }
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Debug"))
            {
                auto showLuaDebug = !mState->luaDebugWindow.getHidden();
                if (ImGui::Checkbox("Lua Debug Window",&showLuaDebug))
                {
                    mState->luaDebugWindow.setHidden(!showLuaDebug);
                }

                if (pRuntime)
                {
                    auto active = pRuntime->getActiveSceneRuntime();
                    auto physicsDebug = active ? active->getPhysicsDebug() : false;
                    if (ImGui::Checkbox("Physics Debug",&physicsDebug))
                    {
                       if (active)
                       {
                           active->setPhysicsDebug(physicsDebug);
                           dynamic_cast<SceneDefinition*>(active->getDefinition())->setPhysicsDebug(physicsDebug);
                       }

                    }
                }

                if(ImGui::BeginMenu("Engine Logging"))
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

        static ImGuiFs::Dialog newDlg;
        newDlg.chooseFolderDialog(newButtonClicked,mState->lastDirectory.c_str());
        if (strlen(newDlg.getChosenPath())>0)
        {
            mState->lastDirectory = newDlg.getChosenPath();
            /*
                auto path = newDlg.getChosenPath();
                ProjectDirectoryModel dirModel();
                mState->project->clear();
            */
        }

        bool openProjectFailed = false;
        static ImGuiFs::Dialog openDlg;
        const char* chosenPath = openDlg.chooseFolderDialog(openButtonClicked,mState->lastDirectory.c_str());
        if (strlen(chosenPath) > 0)
        {
            auto projectDir = openDlg.getChosenPath();
            mState->lastDirectory = projectDir;
            log->error("Opening project {}",projectDir);
            if(mState->project->openFromDirectory(projectDir))
            {
                mState->project->createProjectRuntime();
            }
            else
            {
                openProjectFailed = true;
            }
        }

        if (openProjectFailed)
        {
            ImGui::OpenPopup("Failed to open Project");
        }

        if (ImGui::BeginPopupModal("Failed to open Project", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("This directory does not contain a valid Dream Project\n\n");
            ImGui::Separator();

            if (ImGui::Button("OK",ImVec2(-1,0)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }

        if (showPleaseDestroyScenesDialog)
        {
           ImGui::OpenPopup("Loaded Scenes");
        }

        if(ImGui::BeginPopupModal("Loaded Scenes", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text
            (
                "There are one or more Loaded Scenes.\n"
                "\n"
                "Please destroy them before clearing Caches.\n"
                "\n"
            );
            ImGui::Separator();

            if (ImGui::Button("OK##clearCachesDialog", ImVec2(-1, 0)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }

        if (showQuitDialog)
        {
            ImGui::OpenPopup("Quit?");
        }

        if (ImGui::BeginPopupModal("Quit?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
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
                mState->MainLoopDone = true;
            }

            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }

        if (showSaveSuccessDialog)
        {
            ImGui::OpenPopup("Save Success");
        }

        if (ImGui::BeginPopupModal("Save Success", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Project Saved Successfully!");
            ImGui::Separator();

            if (ImGui::Button("OK",ImVec2(-1,0)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }
    }
}

#include "MenuBar.h"
#include "ScriptDebugWindow.h"
#include "ProjectBrowser.h"
#include "PropertiesWindow.h"
#include "SceneStateWindow.h"
#include "DTContext.h"

#include <DreamCore.h>
#include <imgui_internal.h>
#include <nfd.h>
#include <sstream>



namespace DreamTool
{
    MenuBar::MenuBar(DTContext* def)
        : ImGuiWidget(def)
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
        bool showQuitDialog = false;
        bool newButtonClicked = false;
        bool openButtonClicked = false;
        bool showPleaseDestroyScenesDialog = false;
        bool modelBatchImportClicked = false;
        bool assetCleanupClicked = false;

        bool hasProject = mState->project != nullptr;

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                newButtonClicked = ImGui::MenuItem("New");
                openButtonClicked = ImGui::MenuItem("Open");
                if (hasProject)
                {
                    if(ImGui::MenuItem("Save"))
                    {
                        if(mState->projectDirectory.saveProject())
                        {
                            setMessageString("Saved Successfully");
                        }
                    }
                    ImGui::Separator();
                    if(ImGui::MenuItem("Close"))
                    {
                        mState->closeProject();
                        hasProject = false;
                    }
                }
                ImGui::Separator();
                showQuitDialog = ImGui::MenuItem("Quit");
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                bool showProjectBrowser = mState->projectBrowser.getVisible();
                if (hasProject)
                {
                    if(ImGui::Checkbox("Project Browser",&showProjectBrowser))
                    {
                        mState->projectBrowser.setVisible(showProjectBrowser);
                    }

                    bool showPropertiesWindow = mState->propertiesWindow.getVisible();
                    if(ImGui::Checkbox("Properties Window",&showPropertiesWindow))
                    {
                        mState->propertiesWindow.setVisible(showPropertiesWindow);
                    }

                    bool showSceneStatesWindow = mState->sceneStateWindow.getVisible();
                    if (ImGui::Checkbox("Scene States",&showSceneStatesWindow))
                    {
                        mState->sceneStateWindow.setVisible(showSceneStatesWindow);
                    }

                    bool showGridPropsWindow = mState->gridPropertiesWindow.getVisible();
                    if (ImGui::Checkbox("Tool Properties",&showGridPropsWindow))
                    {
                        mState->gridPropertiesWindow.setVisible(showGridPropsWindow);
                    }
                }

                ImGui::DragFloat("Text Scaling", &(ImGui::GetCurrentContext()->Font->Scale),0.1f,1.0f,10.0f);

                ImGui::EndMenu();
            }

            if (hasProject)
            {
                auto pRuntime = mState->project->getRuntime();

                if (ImGui::BeginMenu("Scene"))
                {
                    vector<string> sceneNames;

                    ProjectDefinition* projDef = nullptr;
                    if (mState->project)
                    {
                        projDef = mState->project->getDefinition();
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
                            setMessageString("Added Scene Runtime: "+newSceneRT->getName());
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
                                pRuntime->setSceneRuntimeAsActive(rt->getUuid());
                                setMessageString("Activated Scene: "+rt->getName());
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
                                setMessageString("Destroyed Scene: "+rt->getName());
                            }
                            mState->selectionHighlighter.clearSelection();
                        }
                    }

                    ImGui::EndMenu();
                }

                if(ImGui::BeginMenu("Components"))
                {
                    if (ImGui::BeginMenu("Input"))
                    {
                        DTContext::InputTarget mode = mState->inputTarget;
                        if (ImGui::RadioButton("To Editor",mode == DTContext::InputTarget::EDITOR))
                        {
                            mState->inputTarget = DTContext::InputTarget::EDITOR;
                        }

                        if (ImGui::RadioButton("To Scene", mode == DTContext::InputTarget::SCENE))
                        {
                            mState->inputTarget = DTContext::InputTarget::SCENE;
                        }
                        ImGui::EndMenu();
                    }

                    float volume = 1.0f;
                    auto rt = mState->project->getRuntime();
                    auto audioComp = rt->getAudioComponent();
                    if (audioComp)
                    {
                        volume = audioComp->getVolume();
                    }

                    if(ImGui::SliderFloat("Volume",&volume,0.0f,1.0f))
                    {
                        if (audioComp)
                        {
                            audioComp->setVolume(volume);
                        }
                    }

                    bool scripting = false;
                    if (pRuntime)
                    {
                        scripting = pRuntime->getScriptComponent()->getEnabled();
                    }
                    if(ImGui::Checkbox("Scripting",&scripting))
                    {
                        if(pRuntime)
                        {
                            pRuntime->getScriptComponent()->setEnabled(scripting);
                        }
                    }

                    bool physics = false;
                    if (pRuntime)
                    {
                        physics = pRuntime->getPhysicsComponent()->getEnabled();
                    }
                    if(ImGui::Checkbox("Physics",&physics))
                    {
                        if(pRuntime)
                        {
                            pRuntime->getPhysicsComponent()->setEnabled(physics);
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

                if (ImGui::BeginMenu("Tools"))
                {
                    if (ImGui::MenuItem("Model Batch Importer"))
                    {
                        modelBatchImportClicked = true;
                    }

                    if(ImGui::MenuItem("Clean Up Assets Directory"))
                    {
                        assetCleanupClicked = true;
                    }
                    ImGui::EndMenu();
                }
            }

            if (ImGui::BeginMenu("Debug"))
            {
                if (hasProject)
                {
                    bool showRenderingPipeline = mState->renderPipelineWindow.getVisible();
                    if (ImGui::Checkbox("Rendering Pipeline",&showRenderingPipeline))
                    {
                        mState->renderPipelineWindow.setVisible(showRenderingPipeline);
                    }

                    bool showJoystickDebug = mState->gamepadStateWindow.getVisible();
                    if (ImGui::Checkbox("Joystick State",&showJoystickDebug))
                    {
                        mState->gamepadStateWindow.setVisible(showJoystickDebug);
                    }

                    auto showLuaDebug = mState->scriptDebugWindow.getVisible();
                    if (ImGui::Checkbox("Script Debug",&showLuaDebug))
                    {
                        mState->scriptDebugWindow.setVisible(showLuaDebug);
                    }

                    bool showCacheContents = mState->cacheContentWindow.getVisible();
                    if (ImGui::Checkbox("Cache Contents",&showCacheContents))
                    {
                        mState->cacheContentWindow.setVisible(showCacheContents);
                    }

                    bool showTaskManager = mState->taskManagerWindow.getVisible();
                    if (ImGui::Checkbox("Task Manager",&showTaskManager))
                    {
                        mState->taskManagerWindow.setVisible(showTaskManager);
                    }


                    auto pRuntime = mState->project->getRuntime();
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
                }

                if(ImGui::BeginMenu("Engine Logging"))
                {
                    spdlog::level::level_enum mode =  LOG_GET_LEVEL();
                    if (ImGui::RadioButton("Off", mode == LOG_LEVEL_OFF))
                    {
                        mode = LOG_LEVEL_OFF;
                    }
                    if (ImGui::RadioButton("Critical", mode == LOG_LEVEL_CRITICAL))
                    {
                        mode = LOG_LEVEL_CRITICAL;
                    }
                    if (ImGui::RadioButton("Error", mode == LOG_LEVEL_ERROR))
                    {
                        mode = LOG_LEVEL_ERROR;
                    }
                    if (ImGui::RadioButton("Warning", mode == LOG_LEVEL_WARN))
                    {
                        mode = LOG_LEVEL_WARN;
                    }
                    if (ImGui::RadioButton("Info", mode == LOG_LEVEL_INFO))
                    {
                        mode = LOG_LEVEL_INFO;
                    }
                    if (ImGui::RadioButton("Debug", mode == LOG_LEVEL_DEBUG))
                    {
                        mode = LOG_LEVEL_DEBUG;
                    }
                    if (ImGui::RadioButton("Trace", mode == LOG_LEVEL_TRACE))
                    {
                        mode = LOG_LEVEL_TRACE;
                    }
                    LOG_LEVEL(mode);
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }

            static char msgBuf[128] = {0};
            snprintf(msgBuf,128,"%s | Input to %s",
                     mMessageString.c_str(),
                     (mState->inputTarget==DTContext::InputTarget::EDITOR?"Editor":"Scene"));

            auto maxX = ImGui::GetWindowContentRegionMax().x;
            ImVec2 msgSize = ImGui::CalcTextSize(msgBuf);

            ImGui::SameLine(maxX-msgSize.x-mMessagePadding);
            ImGui::Text("%s", msgBuf);
            ImGui::EndMainMenuBar();
        }

        bool newProjectFailed = false;
        // New Project
        if (newButtonClicked)
        {
            nfdchar_t *selected_file_path = NULL;
            nfdresult_t result = NFD_PickFolder(&selected_file_path, mState->lastDirectory.c_str());

            if ( result == NFD_OKAY )
            {
                LOG_INFO("MenuBar: Success! {}",selected_file_path);
                if (!mState->newProject(selected_file_path))
                {
                    LOG_INFO("MenuBar: Success! {}",selected_file_path);
                }
                NFD_FreePath(selected_file_path);
            }
            else if ( result == NFD_CANCEL )
            {
                LOG_DEBUG("MenuBar: User pressed cancel.");
                newProjectFailed = true;
            }
            else
            {
                LOG_ERROR("MenuBar: Error: %s\n", NFD_GetError() );
                newProjectFailed = true;
            }
        }

        // Open Project
        bool openProjectFailed = false;

        if (openButtonClicked)
        {
            nfdchar_t *selected_file_path = NULL;
            nfdresult_t result = NFD_PickFolder(&selected_file_path, mState->lastDirectory.c_str());

            if ( result == NFD_OKAY )
            {
                LOG_INFO("MenuBar: Success! {}",selected_file_path);
                openProjectFailed = !mState->openProject(selected_file_path);
                NFD_FreePath(selected_file_path);
            }
            else if ( result == NFD_CANCEL )
            {
                LOG_DEBUG("MenuBar: User pressed cancel.");
                openProjectFailed = true;
            }
            else
            {
                LOG_ERROR("MenuBar: Error: %s\n", NFD_GetError() );
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

        if (assetCleanupClicked)
        {
            ImGui::OpenPopup("Asset Cleanup");
        }

        if (modelBatchImportClicked)
        {
            nfdchar_t *selected_file_path = NULL;
            nfdresult_t result = NFD_OpenDialog(&selected_file_path, nullptr, 0, mState->lastDirectory.c_str());

            if ( result == NFD_OKAY )
            {
                LOG_INFO("MenuBar: Success! {}",selected_file_path);
                mState->lastDirectory = selected_file_path;
                mState->modelDefinitionBatchImporter.setDirectory(selected_file_path);
                mState->modelDefinitionBatchImporter.findModels();
                NFD_FreePath(selected_file_path);
            }
            else if ( result == NFD_CANCEL )
            {
                LOG_DEBUG("MenuBar: User pressed cancel.");
                newProjectFailed = true;
            }
            else
            {
                LOG_ERROR("MenuBar: Error: %s\n", NFD_GetError() );
                newProjectFailed = true;
            }
        }

        if (mState->modelDefinitionBatchImporter.hasModels())
        {
            ImGui::OpenPopup("Batch Import Models");
        }

        if(ImGui::BeginPopupModal("Batch Import Models"))
        {
            ImGui::Columns(2);

            ImGui::BeginChild("ModelsList");
            {
                ImGui::Columns(2);
                ImGui::Text("Model");
                ImGui::NextColumn();
                ImGui::Text("Import");
                ImGui::NextColumn();
                ImGui::Separator();

                int index = 0;
                for (string modelName : mState->modelDefinitionBatchImporter.getModelsFoundNames())
                {
                    ImGui::Text("%s",modelName.c_str());
                    ImGui::NextColumn();
                    ImGui::PushID(index);
                    ImGui::Checkbox("##",(mState->modelDefinitionBatchImporter.getModelsToImport()+index));
                    ImGui::PopID();
                    ImGui::NextColumn();
                    index++;
                }
                ImGui::Columns(1);
            }
            ImGui::EndChild();

            ImGui::NextColumn();
            ImGui::Text("Options");
            bool appendDirName = mState->modelDefinitionBatchImporter.getAppendDirectoryName();
            stringstream appendLabel;
            appendLabel << "Append Directory Name (";
            appendLabel << mState->modelDefinitionBatchImporter.getDirectoryName();
            appendLabel << ")";
            if (ImGui::Checkbox(appendLabel.str().c_str(),&appendDirName))
            {
                mState->modelDefinitionBatchImporter.setAppendDirectoryName(appendDirName);
            }

            auto replace = mState->modelDefinitionBatchImporter.getReplaceExisting();
            if(ImGui::Checkbox("Replace Existing",&replace))
            {
                mState->modelDefinitionBatchImporter.setReplaceExisting(replace);
            }
            if(ImGui::Button("Cancel"))
            {
                mState->modelDefinitionBatchImporter.clear();
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if(ImGui::Button("Import Models"))
            {
                mState->modelDefinitionBatchImporter.import();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        if (mState->modelDefinitionBatchImporter.hasResults())
        {
            ImGui::OpenPopup("Model Import Results");
        }

        if (ImGui::BeginPopupModal("Model Import Results"))
        {
            ImGui::Columns(2);
            ImGui::Text("Model");
            ImGui::NextColumn();
            ImGui::Text("Result");
            ImGui::Separator();
            ImGui::NextColumn();
            for (auto result : mState->modelDefinitionBatchImporter.getImportResults())
            {
                ImGui::Text("%s",result.definition->getName().c_str());
                ImGui::NextColumn();
                ImGui::Text("%s",ModelDefinitionBatchImporter::resultString(result.result).c_str());
                ImGui::NextColumn();
            }
            ImGui::Columns(1);
            ImGui::Separator();
            if(ImGui::Button("OK"))
            {
                mState->modelDefinitionBatchImporter.clear();
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
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

        if(ImGui::BeginPopupModal("Asset Cleanup",nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Directories Removed");
            ImGui::Separator();
            auto subdirsRemoved = mState->projectDirectory.cleanupAssetsDirectory();
            if (subdirsRemoved.empty())
            {
                ImGui::Text("None to remove.");
            }
            else
            {
                for (uint32_t sdir : subdirsRemoved)
                {
                    ImGui::Text("%d",sdir);
                }
            }
            ImGui::Separator();
            if(ImGui::Button("OK"))
            {
                subdirsRemoved.clear();
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }

    float MenuBar::getMessagePadding() const
    {
        return mMessagePadding;
    }

    void MenuBar::setMessagePadding(float messagePadding)
    {
        mMessagePadding = messagePadding;
    }

    string MenuBar::getMessageString() const
    {
        return mMessageString;
    }

    void MenuBar::setMessageString(const string& messageString)
    {
        mMessageString = messageString;
    }

}

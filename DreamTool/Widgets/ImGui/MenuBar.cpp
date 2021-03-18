#include "MenuBar.h"

#include "InputTarget.h"

#include "ScriptDebugWindow.h"
#include "ProjectBrowser.h"
#include "PropertiesWindow.h"
#include "SceneStateWindow.h"
#include "DreamToolContext.h"

#include <DreamCore.h>
#include <imgui_internal.h>
#include <nfd.h>
#include <sstream>

namespace octronic::dream::tool
{
    MenuBar::MenuBar(DreamToolContext* def)
        : ImGuiWidget(def),
          mOpenProjectFailed(false),
          mNewProjectFailed(false),
          mFileQuitClicked(false),
          mFilePreferencesClicked(false),
          mSaveProjectFailed(false),
          mSaveProjectSuccess(false),
          mMessagePadding(15.0f)
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
        auto project = mContext->getProject();

        if (ImGui::BeginMainMenuBar())
        {
            drawFileMenu();
            drawViewMenu();

            if (project)
            {
                drawSceneMenu();
                drawComponentsMenu();
                drawToolsMenu();
            }

            drawDebugMenu();
            drawStatusText();

            ImGui::EndMainMenuBar();
        }

        checkNewProjectFailed();
        checkSaveProject();
        checkOpenProjectFailed();

        checkFilePreferencesClicked();
        checkFileQuitClicked();

    }

    void MenuBar::checkOpenProjectFailed()
    {
        if (mOpenProjectFailed)
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
    }

    void MenuBar::checkSaveProject()
    {
        if (mSaveProjectFailed)
        {
            ImGui::OpenPopup("Failed to Save Project");
        }

        if (ImGui::BeginPopupModal("Failed to Save Project", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Error while saving project");
            ImGui::Separator();

            if (ImGui::Button("Dang",ImVec2(-1,0)))
            {
                mSaveProjectFailed = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }

        if (mSaveProjectSuccess)
        {
            ImGui::OpenPopup("Saved Project");
        }

        if (ImGui::BeginPopupModal("Saved Project", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Project Saved Successfuly");

            ImGui::Separator();

            if (ImGui::Button("Nice",ImVec2(-1,0)))
            {
                mSaveProjectSuccess = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::SetItemDefaultFocus();
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

    void MenuBar::drawStatusText()
    {
        static char msgBuf[128] = {0};
        snprintf(msgBuf,128, "%s | Input to %s",
                 mMessageString.c_str(),
                 mContext->getInputTarget() == InputTarget::INPUT_TARGET_EDITOR ? "Editor":"Scene");

        auto maxX = ImGui::GetWindowContentRegionMax().x;
        ImVec2 msgSize = ImGui::CalcTextSize(msgBuf);

        ImGui::SameLine(maxX-msgSize.x-mMessagePadding);
        ImGui::Text("%s", msgBuf);
    }

    void MenuBar::drawFileMenu()
    {
        auto project = mContext->getProject();

        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Preferences")) mFilePreferencesClicked = true;
            ImGui::Separator();
            if (ImGui::MenuItem("New"))	onFileNewClicked();
            if (ImGui::MenuItem("Open")) onFileOpenClicked();

            if (project)
            {
                if (ImGui::MenuItem("Save"))
                {
                    if(mContext->getProjectDirectory()->saveProject())
                    {
                        mSaveProjectSuccess = true;
                    }
                    else
                    {
                        mSaveProjectFailed = true;
                    }
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Close"))
                {
                    mContext->closeProject();
                }
            }
            ImGui::Separator();
            mFileQuitClicked = ImGui::MenuItem("Quit");
            ImGui::EndMenu();
        }
    }

    void MenuBar::onFileNewClicked()
    {
        nfdchar_t *selected_file_path = nullptr;
        nfdresult_t result = NFD_PickFolder(&selected_file_path, mContext->getLastDirectory().c_str());

        if (result == NFD_OKAY)
        {
            LOG_INFO("MenuBar: Success! {}",selected_file_path);
            if (!mContext->newProject(selected_file_path))
            {
                LOG_INFO("MenuBar: Success! {}",selected_file_path);
            }
            NFD_FreePath(selected_file_path);
        }
        else if (result == NFD_CANCEL)
        {
            LOG_DEBUG("MenuBar: User pressed cancel.");
            mNewProjectFailed = true;
        }
        else
        {
            LOG_ERROR("MenuBar: Error: %s\n", NFD_GetError() );
            mNewProjectFailed = true;
        }
    }

    void MenuBar::onFileOpenClicked()
    {
        // Open Project
        nfdchar_t *selected_file_path = nullptr;
        nfdresult_t result = NFD_PickFolder(&selected_file_path, mContext->getLastDirectory().c_str());

        if (result == NFD_OKAY)
        {
            LOG_INFO("MenuBar: Success! {}",selected_file_path);
            mOpenProjectFailed = !mContext->openProject(selected_file_path);
            NFD_FreePath(selected_file_path);
        }
        else if (result == NFD_CANCEL)
        {
            LOG_DEBUG("MenuBar: User pressed cancel.");
            mOpenProjectFailed = true;
        }
        else
        {
            LOG_ERROR("MenuBar: Error: %s\n", NFD_GetError() );
            mOpenProjectFailed = true;
        }
    }

    void MenuBar::checkFilePreferencesClicked()
    {
        PreferencesModel* pm = mContext->getPreferencesModel();
        if (mFilePreferencesClicked)
        {
            ImGui::OpenPopup("Preferences");

            if (ImGui::BeginPopupModal("Preferences", nullptr))
            {
                ImGui::Columns(3);
                ImGui::Text("External Editor: ");
                ImGui::NextColumn();
                ImGui::Text("%s",pm->getExternalEditorPath().c_str());
                ImGui::NextColumn();
                if (ImGui::Button("Choose External Editor..."))
                {
                    nfdchar_t *selected_path = nullptr;
                    nfdresult_t result = NFD_OpenDialogN(&selected_path, nullptr,0, mContext->getLastDirectory().c_str());

                    if (result == NFD_OKAY)
                    {
                        pm->setExternalEditorPath(selected_path);
                        NFD_FreePath(selected_path);
                    }
                    else if (result == NFD_CANCEL)
                    {
                        LOG_ERROR("MenuBar: Select External Editor Cancelled", NFD_GetError() );
                    }
                    else
                    {
                        LOG_ERROR("MenuBar: Select External Editor Error: %s", NFD_GetError() );
                    }
                }

                ImGui::Columns(1);

                ImGui::Separator();

                if (ImGui::Button("Save##preferencesSave", ImVec2(120, 0)))
                {
                    mFilePreferencesClicked = false;
                    pm->save();
                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetItemDefaultFocus();
                ImGui::EndPopup();
            }
        }
    }

    void MenuBar::checkNewProjectFailed()
    {
        if (mNewProjectFailed)
        {
            ImGui::OpenPopup("New Project Failed");

            if (ImGui::BeginPopupModal("New Project Failed", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Error, Creating new project failed");

                ImGui::Separator();

                if (ImGui::Button("OK##confirmNewFailed", ImVec2(120, 0)))
                {
                    mNewProjectFailed = false;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::SetItemDefaultFocus();
                ImGui::EndPopup();
            }
        }

    }

    void MenuBar::checkFileQuitClicked()
    {
        if (mFileQuitClicked)
        {
            ImGui::OpenPopup("Quit?");

            if (ImGui::BeginPopupModal("Quit?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Are you sure you want to quit?\n\nAny unsaved changes will be lost.\n\n");
                ImGui::Separator();

                if (ImGui::Button("Cancel##cancelQuit", ImVec2(120, 0)))
                {
                    mFileQuitClicked = false;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::SameLine();

                if (ImGui::Button("Quit##confirmQuit", ImVec2(120, 0)))
                {
                    mFileQuitClicked = false;
                    ImGui::CloseCurrentPopup();
                    mContext->setMainLoopDone(true);
                }

                ImGui::SetItemDefaultFocus();
                ImGui::EndPopup();
            }
        }
    }

    void MenuBar::drawViewMenu()
    {
        if (ImGui::BeginMenu("View"))
        {
            auto project = mContext->getProject();

            if (project)
            {
                bool showProjectBrowser = mContext->getProjectBrowser()->getVisible();
                if(ImGui::Checkbox("Project Browser",&showProjectBrowser))
                {
                    mContext->getProjectBrowser()->setVisible(showProjectBrowser);
                }

                bool showAssetBrowser = mContext->getAssetBrowser()->getVisible();
                if(ImGui::Checkbox("Asset Browser",&showAssetBrowser))
                {
                    mContext->getAssetBrowser()->setVisible(showAssetBrowser);
                }

                bool showPropertiesWindow = mContext->getPropertiesWindow()->getVisible();
                if(ImGui::Checkbox("Properties Window",&showPropertiesWindow))
                {
                    mContext->getPropertiesWindow()->setVisible(showPropertiesWindow);
                }

                bool showSceneStatesWindow = mContext->getSceneStateWindow()->getVisible();
                if (ImGui::Checkbox("Scene States",&showSceneStatesWindow))
                {
                    mContext->getSceneStateWindow()->setVisible(showSceneStatesWindow);
                }

                bool showGridPropsWindow = mContext->getToolPropertiesWindow()->getVisible();
                if (ImGui::Checkbox("Tool Properties",&showGridPropsWindow))
                {
                    mContext->getToolPropertiesWindow()->setVisible(showGridPropsWindow);
                }

                bool showGlPreviewWindow = mContext->getGlPreviewWindowComponent()->getVisible();
                if (ImGui::Checkbox("GL Preview", &showGlPreviewWindow))
                {
                    mContext->getGlPreviewWindowComponent()->setVisible(showGlPreviewWindow);
                }
            }

            ImGui::DragFloat("Text Scaling", &(ImGui::GetCurrentContext()->Font->Scale),0.1f,1.0f,10.0f);

            ImGui::EndMenu();
        }
    }

    void MenuBar::drawComponentsMenu()
    {
        auto project = mContext->getProject();

        bool showPleaseDestroyScenesDialog = false;

        if(ImGui::BeginMenu("Components"))
        {
            if (ImGui::BeginMenu("Input"))
            {
                InputTarget mode = mContext->getInputTarget();
                if (ImGui::RadioButton("To Editor",mode == InputTarget::INPUT_TARGET_EDITOR))
                {
                    mContext->setInputTarget(InputTarget::INPUT_TARGET_EDITOR);
                }

                if (ImGui::RadioButton("To Scene", mode == InputTarget::INPUT_TARGET_SCENE))
                {
                    mContext->setInputTarget(InputTarget::INPUT_TARGET_SCENE);
                }
                ImGui::EndMenu();
            }

            float volume = 1.0f;
            auto rt = project->getRuntime();
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

            auto pRuntime = project->getRuntime();

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

        if (showPleaseDestroyScenesDialog)
        {
            ImGui::OpenPopup("Loaded Scenes");
        }

        if(ImGui::BeginPopupModal("Loaded Scenes", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text(
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
    }

    void MenuBar::drawToolsMenu()
    {
        bool modelBatchImportClicked = false;
        static bool assetCleanupClicked = false;

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

        if (assetCleanupClicked)
        {
            ImGui::OpenPopup("Asset Cleanup");
        }

        static bool cleanupComplete = false;

        if(ImGui::BeginPopupModal("Asset Cleanup",nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Directories Removed");
            ImGui::Separator();
            static vector<string> subdirsRemoved;
            if (!cleanupComplete)
            {
                subdirsRemoved = mContext->getProjectDirectory()->cleanupAssetsDirectory();
                cleanupComplete = true;
            }
            else
            {
                if (subdirsRemoved.empty())
                {
                    ImGui::Text("None to remove.");
                }
                else
                {
                    for (string& sdir : subdirsRemoved)
                    {
                        ImGui::Text("%s",sdir.c_str());
                    }
                }
            }
            ImGui::Separator();
            if(ImGui::Button("OK"))
            {
                cleanupComplete = false;
                assetCleanupClicked = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        if (modelBatchImportClicked)
        {
            nfdchar_t *selected_file_path = nullptr;
            nfdresult_t result = NFD_OpenDialog(&selected_file_path, nullptr, 0, mContext->getLastDirectory().c_str());

            if ( result == NFD_OKAY )
            {
                LOG_INFO("MenuBar: Success! {}",selected_file_path);
                mContext->setLastDirectory(selected_file_path);
                mContext->getModelDefinitionBatchImporter()->setDirectory(selected_file_path);
                mContext->getModelDefinitionBatchImporter()->findModels();
                NFD_FreePath(selected_file_path);
            }
            else if ( result == NFD_CANCEL )
            {
                LOG_DEBUG("MenuBar: User pressed cancel.");
            }
            else
            {
                LOG_ERROR("MenuBar: Error: %s\n", NFD_GetError() );
            }
        }

        if (mContext->getModelDefinitionBatchImporter()->hasModels())
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
                for (string& modelName : mContext->getModelDefinitionBatchImporter()->getModelsFoundNames())
                {
                    ImGui::Text("%s",modelName.c_str());
                    ImGui::NextColumn();
                    ImGui::PushID(index);
                    ImGui::Checkbox("##",(mContext->getModelDefinitionBatchImporter()->getModelsToImport()+index));
                    ImGui::PopID();
                    ImGui::NextColumn();
                    index++;
                }
                ImGui::Columns(1);
            }
            ImGui::EndChild();

            ImGui::NextColumn();
            ImGui::Text("Options");
            bool appendDirName = mContext->getModelDefinitionBatchImporter()->getAppendDirectoryName();
            stringstream appendLabel;
            appendLabel << "Append Directory Name (";
            appendLabel << mContext->getModelDefinitionBatchImporter()->getDirectoryName();
            appendLabel << ")";
            if (ImGui::Checkbox(appendLabel.str().c_str(),&appendDirName))
            {
                mContext->getModelDefinitionBatchImporter()->setAppendDirectoryName(appendDirName);
            }

            auto replace = mContext->getModelDefinitionBatchImporter()->getReplaceExisting();
            if(ImGui::Checkbox("Replace Existing",&replace))
            {
                mContext->getModelDefinitionBatchImporter()->setReplaceExisting(replace);
            }
            if(ImGui::Button("Cancel"))
            {
                mContext->getModelDefinitionBatchImporter()->clear();
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if(ImGui::Button("Import Models"))
            {
                mContext->getModelDefinitionBatchImporter()->import();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        if (mContext->getModelDefinitionBatchImporter()->hasResults())
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
            for (auto result : mContext->getModelDefinitionBatchImporter()->getImportResults())
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
                mContext->getModelDefinitionBatchImporter()->clear();
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }

    void MenuBar::drawSceneMenu()
    {
        auto project = mContext->getProject();
        shared_ptr<ProjectDefinition> projDef;
        shared_ptr<ProjectRuntime> pRuntime;

        if (ImGui::BeginMenu("Scene"))
        {
            vector<string> sceneNames;

            if (project)
            {
                projDef = project->getDefinition();
                pRuntime = project->getRuntime();

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
            if (StringCombo("Load Scene", &sceneToLoadIndex, sceneNames, sceneNames.size()))
            {
                if (projDef)
                {
                    auto selectedSceneDef = projDef->getSceneDefinitionAtIndex(sceneToLoadIndex);
                    if (!pRuntime->hasSceneRuntime(selectedSceneDef->getUuid()))
                    {
                        auto newSceneRT = make_shared<SceneRuntime>(pRuntime, selectedSceneDef);
                        if (newSceneRT->init())
                        {
							newSceneRT->setState(SceneState::SCENE_STATE_TO_LOAD);
							pRuntime->addSceneRuntime(newSceneRT);
							setMessageString("Added Scene Runtime: "+newSceneRT->getName());
                        }
                        else
                        {
							setMessageString("Scene Init Failed: "+newSceneRT->getName());
                        }
                    }
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
            if (StringCombo("Set Active Scene", &sceneActiveIndex, runtimeNames, runtimeNames.size()))
            {
                if (pRuntime)
                {
                    auto rt = pRuntime->getSceneRuntimeVector().at(sceneActiveIndex);
                    if (rt && rt->hasState(SCENE_STATE_LOADED))
                    {
                        rt->setState(SCENE_STATE_ACTIVE);
                        pRuntime->setActiveSceneRuntime(rt->getUuid());
                        setMessageString("Activated Scene: "+rt->getName());
                    }
                }
            }

            int sceneToDestroyIndex = -1;
            if (StringCombo("Set Scene \"To Destroy\"", &sceneToDestroyIndex, runtimeNames, runtimeNames.size()))
            {
                mContext->getPropertiesWindow()->clearPropertyTargets();
                if (pRuntime)
                {
                    auto rt = pRuntime->getSceneRuntimeVector().at(sceneToDestroyIndex);
                    if (rt)
                    {
                        rt->setState(SceneState::SCENE_STATE_TO_DESTROY);
                        setMessageString("Destroying Scene: "+rt->getName());
                    }
                    mContext->getSelectionHighlighter()->clearSelection();
                }
            }

            ImGui::EndMenu();
        }
    }

    void MenuBar::drawDebugMenu()
    {
        auto project = mContext->getProject();

        if (ImGui::BeginMenu("Debug"))
        {
            if (project)
            {
                bool showRenderingPipeline = mContext->getRenderPipelineWindow()->getVisible();
                if (ImGui::Checkbox("Rendering Pipeline",&showRenderingPipeline))
                {
                    mContext->getRenderPipelineWindow()->setVisible(showRenderingPipeline);
                }

                bool showJoystickDebug = mContext->getGamepadStateWindow()->getVisible();
                if (ImGui::Checkbox("Joystick State",&showJoystickDebug))
                {
                    mContext->getGamepadStateWindow()->setVisible(showJoystickDebug);
                }

                auto showLuaDebug = mContext->getScriptDebugWindow()->getVisible();
                if (ImGui::Checkbox("Script Debug",&showLuaDebug))
                {
                    mContext->getScriptDebugWindow()->setVisible(showLuaDebug);
                }

                bool showCacheContents = mContext->getCacheContentWindow()->getVisible();
                if (ImGui::Checkbox("Cache Contents",&showCacheContents))
                {
                    mContext->getCacheContentWindow()->setVisible(showCacheContents);
                }

                bool showTaskManager = mContext->getTaskManagerWindow()->getVisible();
                if (ImGui::Checkbox("Task Manager",&showTaskManager))
                {
                    mContext->getTaskManagerWindow()->setVisible(showTaskManager);
                }
            }
#ifdef ENABLE_LOGGING
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
#endif
            ImGui::EndMenu();
        }
    }
}

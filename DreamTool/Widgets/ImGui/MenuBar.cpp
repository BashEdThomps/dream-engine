#include "MenuBar.h"

#include "InputTarget.h"

#include "ScriptDebugWindow.h"
#include "ProjectBrowser.h"
#include "PropertiesWindow/PropertiesWindow.h"
#include "SceneStateWindow.h"
#include "DreamToolContext.h"

#include <DreamCore.h>
#include <imgui_internal.h>
#include <nfd.h>
#include <sstream>

namespace octronic::dream::tool
{
  MenuBar::MenuBar(DreamToolContext& def)
    : ImGuiWidget(def),
      mOpenProjectFailed(false),
      mNewProjectFailed(false),
      mSaveProjectFailed(false),
      mSaveProjectSuccess(false),
      mFileQuitClicked(false),
      mFilePreferencesClicked(false),
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

    if (ImGui::BeginMainMenuBar())
    {
      drawFileMenu();
      drawViewMenu();

      auto& pCtxOpt = getContext().getProjectContext();
      if (pCtxOpt)
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
        mOpenProjectFailed = false;
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

      if (ImGui::Button("OK",ImVec2(-1,0)))
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
             getContext().getInputTarget() == InputTarget::INPUT_TARGET_EDITOR ? "Editor":"Scene");

    auto maxX = ImGui::GetWindowContentRegionMax().x;
    ImVec2 msgSize = ImGui::CalcTextSize(msgBuf);

    ImGui::SameLine(maxX-msgSize.x-mMessagePadding);
    ImGui::Text("%s", msgBuf);
  }

  void MenuBar::drawFileMenu()
  {
    auto& pCtxOpt = getContext().getProjectContext();

    if (ImGui::BeginMenu("File"))
    {
      if (ImGui::MenuItem("Preferences")) mFilePreferencesClicked = true;

      ImGui::Separator();

      if (ImGui::MenuItem("New"))	onFileNewClicked();
      if (ImGui::MenuItem("Open")) onFileOpenClicked();

      if (pCtxOpt)
      {
        if (ImGui::MenuItem("Save"))
        {
          auto& pDirOpt = pCtxOpt.value().getProjectDirectory();

          if (pDirOpt)
          {
            auto& pCtx = pCtxOpt.value();
            auto& pDir = pDirOpt.value();
            auto& pDefOpt = pCtx.getProjectDefinition();
            if (pDefOpt)
            {
              auto& pDef = pDefOpt.value();
              if(pDir.saveProject(pDef))
              {
                mSaveProjectSuccess = true;
              }
              else
              {
                mSaveProjectFailed = true;
              }
            }
          }
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Close"))
        {
          getContext().closeProject();
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
    nfdresult_t result = NFD_PickFolder(&selected_file_path, getContext().getLastDirectory().c_str());

    if (result == NFD_OKAY)
    {
      LOG_INFO("MenuBar: Success! {}",selected_file_path);
      if (!getContext().newProject(selected_file_path))
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
    nfdresult_t result = NFD_PickFolder(&selected_file_path, getContext().getLastDirectory().c_str());

    if (result == NFD_OKAY)
    {
      LOG_INFO("MenuBar: Success! {}",selected_file_path);
      mOpenProjectFailed = !getContext().openProject(selected_file_path);
    	NFD_FreePath(selected_file_path);
    }
    else if (result == NFD_CANCEL)
    {
      LOG_DEBUG("MenuBar: User pressed cancel.");
    }
    else
    {
      LOG_ERROR("MenuBar: Error: %s\n", NFD_GetError() );
      mOpenProjectFailed = true;
    }
  }

  void MenuBar::checkFilePreferencesClicked()
  {
    auto& prefsModel = getContext().getPreferencesModel();

    if (mFilePreferencesClicked)
    {
      ImGui::OpenPopup("Preferences");

      if (ImGui::BeginPopupModal("Preferences", nullptr))
      {
        ImGui::Text("External Editor: ");
        ImGui::Text("%s",prefsModel.getExternalEditorPath().c_str());

        if (ImGui::Button("Choose External Editor..."))
        {
          nfdchar_t *selected_path = nullptr;
          nfdresult_t result = NFD_OpenDialogN(&selected_path, nullptr,0, getContext().getLastDirectory().c_str());

          if (result == NFD_OKAY)
          {
            prefsModel.setExternalEditorPath(selected_path);
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
          prefsModel.save();
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
          getContext().setMainLoopDone(true);
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
      bool showProjectBrowser = getContext().getProjectBrowser().getVisible();
      if(ImGui::Checkbox("Project Browser",&showProjectBrowser))
      {
        getContext().getProjectBrowser().setVisible(showProjectBrowser);
      }

      bool showAssetBrowser = getContext().getAssetBrowser().getVisible();
      if(ImGui::Checkbox("Asset Browser",&showAssetBrowser))
      {
        getContext().getAssetBrowser().setVisible(showAssetBrowser);
      }

      bool showPropertiesWindow = getContext().getPropertiesWindow().getVisible();
      if(ImGui::Checkbox("Properties Window",&showPropertiesWindow))
      {
        getContext().getPropertiesWindow().setVisible(showPropertiesWindow);
      }

      bool showSceneStatesWindow = getContext().getSceneStateWindow().getVisible();
      if (ImGui::Checkbox("Scene States",&showSceneStatesWindow))
      {
        getContext().getSceneStateWindow().setVisible(showSceneStatesWindow);
      }

      bool showGridPropsWindow = getContext().getToolPropertiesWindow().getVisible();
      if (ImGui::Checkbox("Tool Properties",&showGridPropsWindow))
      {
        getContext().getToolPropertiesWindow().setVisible(showGridPropsWindow);
      }

      bool showGlPreviewWindow = getContext().getGlPreviewWindowComponent().getVisible();
      if (ImGui::Checkbox("GL Preview", &showGlPreviewWindow))
      {
        getContext().getGlPreviewWindowComponent().setVisible(showGlPreviewWindow);
      }

    	ImGui::DragFloat("Text Scaling", &(ImGui::GetCurrentContext()->Font->Scale),0.1f,1.0f,10.0f);

    	ImGui::EndMenu();

    }
  }


  void
  MenuBar::drawComponentsMenu
  ()
  {
    auto& pCtxOpt = getContext().getProjectContext();

    bool showPleaseDestroyScenesDialog = false;

    if(ImGui::BeginMenu("Components"))
    {
      if (ImGui::BeginMenu("Input"))
      {
        InputTarget mode = getContext().getInputTarget();
        if (ImGui::RadioButton("To Editor",mode == InputTarget::INPUT_TARGET_EDITOR))
        {
          getContext().setInputTarget(InputTarget::INPUT_TARGET_EDITOR);
        }

        if (ImGui::RadioButton("To Scene", mode == InputTarget::INPUT_TARGET_SCENE))
        {
          getContext().setInputTarget(InputTarget::INPUT_TARGET_SCENE);
        }
        ImGui::EndMenu();
      }

      float volume = 1.0f;
      if (pCtxOpt)
      {
        auto& pCtx = pCtxOpt.value();
        auto& pRuntOpt = pCtx.getProjectRuntime();
        if (pRuntOpt)
        {
          auto& pRunt = pRuntOpt.value();
          auto& audioComp = pRunt.getAudioComponent();

          volume = audioComp.getVolume();
          if(ImGui::SliderFloat("Volume",&volume,0.0f,1.0f))
          {
            audioComp.setVolume(volume);
          }

          if (ImGui::MenuItem("Clear Caches"))
          {
            if (!pRunt.hasLoadedScenes())
            {
              pRunt.clearAllCaches();
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
    }
  }

  void MenuBar::drawToolsMenu()
  {
    static bool assetCleanupClicked = false;

    if (ImGui::BeginMenu("Tools"))
    {
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
        auto& pCtxOpt = getContext().getProjectContext();
        if (pCtxOpt)
        {
          auto& pCtx = pCtxOpt.value();
          auto& pDirOpt = pCtx.getProjectDirectory();
          if (pDirOpt)
          {
            /* TODO
          auto& pDir = pDirOpt.value();
          subdirsRemoved = pDir.cleanupAssetsDirectory();
          */
            cleanupComplete = true;
          }
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

    }
  }

  void MenuBar::drawSceneMenu()
  {
    auto& pCtxOpt = getContext().getProjectContext();
    if (pCtxOpt)
    {
      auto& pCtx = pCtxOpt.value();

      if (ImGui::BeginMenu("Scene"))
      {

        auto& pDefOpt = pCtx.getProjectDefinition();
        if (pDefOpt)
        {
          auto& pDef = pDefOpt.value();
          auto& pRuntOpt = pCtx.getProjectRuntime();

          // Definitions Names Vector
          vector<string> sceneDefNames;
          auto scenesVector = pDef.getSceneDefinitionsVector();
          for (auto& sceneWrap : scenesVector)
          {
            auto& scene = sceneWrap.get();
            sceneDefNames.push_back(scene.getName());
          }

          // Load Scene Combo
          int sceneToLoadIndex = -1;
          if (StringCombo("Load Scene", &sceneToLoadIndex, sceneDefNames, sceneDefNames.size()))
          {
            if (pDefOpt && pRuntOpt)
            {
              auto& pDef = pDefOpt.value();
              auto& pRunt = pRuntOpt.value();
              auto& selectedSceneDef = pDef.getSceneDefinitionAtIndex(sceneToLoadIndex);

              if (!pRunt.hasSceneRuntime(selectedSceneDef.getUuid()))
              {
                auto& newSceneRT = pRunt.createSceneRuntime(selectedSceneDef);
                if (newSceneRT.init())
                {
                  newSceneRT.setState(SceneState::SCENE_STATE_TO_LOAD);
                  setMessageString("Added Scene Runtime: "+newSceneRT.getName());
                }
                else
                {
                  setMessageString("Scene Init Failed: "+newSceneRT.getName());
                }
              }
            }
          }

          // Scene Runtime Names
          vector<string> runtimeNames;
          if (pRuntOpt)
          {
            auto& pRunt = pRuntOpt.value();
            for (auto& s : pRunt.getSceneRuntimeVector())
            {
              // Show only loaded scenes
              runtimeNames.push_back(s.get().getName());
            }

            // Activate Scene Combo
            int sceneActiveIndex = -1;
            if (StringCombo("Activate Scene", &sceneActiveIndex, runtimeNames, runtimeNames.size()))
            {
              auto& rt = pRunt.getSceneRuntimeVector().at(sceneActiveIndex).get();
              if (rt.hasState(SCENE_STATE_LOADED))
              {
                rt.setState(SCENE_STATE_ACTIVE);
                pRunt.setActiveSceneRuntime(rt.getUuid());
                setMessageString("Activated Scene: "+rt.getName());
              }
            }

            // Destroy Scene Combo
            int sceneToDestroyIndex = -1;
            if (StringCombo("Destroy Scene \"To Destroy\"", &sceneToDestroyIndex, runtimeNames, runtimeNames.size()))
            {
              getContext().getPropertiesWindow().clearPropertyTargets();
              auto& rt = pRunt.getSceneRuntimeVector().at(sceneToDestroyIndex).get();
              rt.setState(SceneState::SCENE_STATE_TO_DESTROY);
              setMessageString("Destroying Scene: "+rt.getName());
              getContext().getSelectionHighlighter().clearSelection();
            }

            ImGui::EndMenu();
          }
        }
      }
    }
  }

  void MenuBar::drawDebugMenu()
  {
    auto& pCtxOpt = getContext().getProjectContext();
    if (ImGui::BeginMenu("Debug"))
    {
      if (pCtxOpt)
      {
        bool showRenderingPipeline = getContext().getRenderPipelineWindow().getVisible();
        if (ImGui::Checkbox("Rendering Pipeline",&showRenderingPipeline))
        {
          getContext().getRenderPipelineWindow().setVisible(showRenderingPipeline);
        }

        bool showJoystickDebug = getContext().getGamepadStateWindow().getVisible();
        if (ImGui::Checkbox("Joystick State",&showJoystickDebug))
        {
          getContext().getGamepadStateWindow().setVisible(showJoystickDebug);
        }

        auto showLuaDebug = getContext().getScriptDebugWindow().getVisible();
        if (ImGui::Checkbox("Script Debug",&showLuaDebug))
        {
          getContext().getScriptDebugWindow().setVisible(showLuaDebug);
        }

        bool showCacheContents = getContext().getCacheContentWindow().getVisible();
        if (ImGui::Checkbox("Cache Contents",&showCacheContents))
        {
          getContext().getCacheContentWindow().setVisible(showCacheContents);
        }

        bool showTaskManager = getContext().getTaskManagerWindow().getVisible();
        if (ImGui::Checkbox("Task Manager",&showTaskManager))
        {
          getContext().getTaskManagerWindow().setVisible(showTaskManager);
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

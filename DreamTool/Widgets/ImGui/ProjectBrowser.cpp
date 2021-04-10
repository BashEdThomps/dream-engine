#include "ProjectBrowser.h"

#ifdef WIN32
#include <Windows.h>
#endif

#include "PropertiesWindow/PropertiesWindow.h"
#include "DreamToolContext.h"

#include <DreamCore.h>
#include <nfd.h>
#include <sstream>

using std::stringstream;
using std::exception;
using octronic::dream::Constants;

namespace octronic::dream::tool
{
  ProjectBrowser::ProjectBrowser
  (DreamToolContext& project)
    : ImGuiWidget(project,true)
  {}

  void
  ProjectBrowser::draw
  ()
  {
    ImGui::Begin("Project",&mVisible);

    auto& pCtxOpt = getContext().getProjectContext();
    if (pCtxOpt)
    {
      auto& pCtx = pCtxOpt.value();
      auto& pDefOpt = pCtx.getProjectDefinition();

      if (pDefOpt)
      {
        auto& pDef = pDefOpt.value();

        // Project Tree
        ImGui::PushID("ProjectTree");

        int treeID = 0;
        bool projectNodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)++treeID,node_flags,pDef.getName().c_str(),0);

        auto& propsWindow = getContext().getPropertiesWindow();

        if (ImGui::IsItemClicked())
        {
          LOG_TRACE("ProjectBrowser: Project clicked {}", pDef.getName());
          propsWindow.pushPropertyTarget(PropertyType_Project, pDef);
        }

        if (projectNodeOpen)
        {
          int sdTreeID = 0;

          for (auto& sDefWrap : pDef.getSceneDefinitionsVector())
          {
            auto& sDef = sDefWrap.get();
            bool sceneNodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)++sdTreeID, node_flags, sDef.getName().c_str(), 0);

            if (ImGui::IsItemClicked())
            {
              LOG_TRACE("ProjectBrowser: Scene Clicked {}", sDef.getName());
              propsWindow.pushPropertyTarget(PropertyType_Scene, sDef);
            }

            if (sceneNodeOpen)
            {
              auto& rootSo = sDef.getRootSceneEntityDefinition().value();
              addEntityNode(rootSo);
              ImGui::TreePop();
            }
          }

          ImGui::TreePop();
        } // Project Name
        ImGui::PopID();
      }
    }
    else
    {
      ImGui::BulletText("No Project Open");
    }
    ImGui::End();
  }

  void
  ProjectBrowser::addEntityNode
  (SceneEntityDefinition& sceneEntityDef)
  {
    auto& sceneDef = sceneEntityDef.getSceneDefinition();

    int treeId = 0;
    ImGuiTreeNodeFlags flags = (sceneEntityDef.countChildDefinitions() == 0 ? leaf_flags : node_flags);
    ImGui::PushID(sceneEntityDef.getUuid());

    bool isSelected = find_if(mSelectedNodes.begin(), mSelectedNodes.end(),
                              [&](reference_wrapper<Definition>& next){ return next.get().getUuid() == sceneEntityDef.getUuid(); }) != mSelectedNodes.end();

    // Flags
    stringstream nameStr;
    stringstream flagsStr;

    auto& cam = sceneDef.getCameraDefinition();

    if (cam.getCameraEntityUuid() == sceneEntityDef.getUuid())
    {
      if (!flagsStr.str().empty())
      {
        flagsStr << ",";
      }
      flagsStr << "CAM";
    }

    if (flagsStr.str().size() > 0)
    {
      nameStr << "(";
      nameStr << flagsStr.str();
      nameStr << ") ";
    }

    nameStr << sceneEntityDef.getName();
    bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)treeId,
                                      flags | (isSelected ? ImGuiTreeNodeFlags_Selected : 0),
                                      nameStr.str().c_str(), 0);

    // Entity Context Menu
    bool deleteClicked = false;
    bool copyToClicked = false;

    if (ImGui::BeginPopupContextItem())
    {
      string defName = sceneEntityDef.getName();
      if (mSelectedNodes.size() > 1)
      {
        ImGui::Text("%d objects selected",static_cast<int>(mSelectedNodes.size()));
      }
      else
      {
        ImGui::Text("%s",defName.c_str());
      }

      // No Root Deletion
      if (sceneEntityDef.hasParentDefinition())
      {
        // Deletion
        ImGui::Separator();
        char deleteBuffer[buf_sz];
        snprintf(deleteBuffer, buf_sz, "Delete %s",defName.c_str());
        deleteClicked = ImGui::MenuItem(deleteBuffer);
      }

      // Copying
      ImGui::Separator();

      char copySelectedBuffer[buf_sz];
      snprintf(copySelectedBuffer,buf_sz,"Copy selected object(s) into %s",defName.c_str());
      copyToClicked = ImGui::MenuItem(copySelectedBuffer);

      ImGui::EndPopup();
    }

    // Context Menu Items
    if (deleteClicked)
    {
      auto parentOpt = sceneEntityDef.getParentDefinition();

      if (parentOpt)
      {
        auto& parent = parentOpt.value().get();
        parent.removeChildDefinition(sceneEntityDef);
      }

      mSelectedNodes.clear();
    }
    else if (copyToClicked)
    {
      for (auto& node : mSelectedNodes)
      {
        auto& defToCreate = static_cast<SceneEntityDefinition&>(node.get());
        sceneEntityDef.createChildDefinitionFrom(defToCreate);
      }
      mSelectedNodes.clear();
    }

    // Node Selection
    if (ImGui::IsItemClicked())
    {
      if (ImGui::GetIO().KeyCtrl)
      {
        // CTRL+click to toggle
        mSelectedNodes.push_back(sceneEntityDef);
      }
      else
      {
        // Click to single-select or CTRL+click to toggle
        mSelectedNodes.clear();
        mSelectedNodes.push_back(sceneEntityDef);
      }

      //getContext().getSelectionHighlighter().setSelectedEntity(sceneEntityDef);
      LOG_TRACE("ProjectBrowser: Entity Clicked {}",sceneEntityDef.getName());
      getContext().getPropertiesWindow().pushPropertyTarget(PropertyType_SceneEntity, sceneEntityDef);
    }

    // Drag Source
    if (sceneEntityDef.hasParentDefinition() && ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
      mDragDropSource.selfDefinition = sceneEntityDef;
      mDragDropSource.parentDefinition = sceneEntityDef.getParentDefinition();

      ImGui::SetDragDropPayload( Constants::SCENE_ENTITY_DEFINITION.c_str(),
                                 &mDragDropSource, sizeof(SceneEntityDragDropSource*));
      ImGui::Text("Reparent %s",sceneEntityDef.getName().c_str());
      ImGui::EndDragDropSource();
    }

    // Drop Target
    if (ImGui::BeginDragDropTarget())
    {
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Constants::SCENE_ENTITY_DEFINITION.c_str()))
      {
        IM_ASSERT(payload->DataSize == sizeof(SceneEntityDragDropSource*));

        auto& sourceDefinition = mDragDropSource.selfDefinition.value().get();

        LOG_TRACE("ProjectBrowser: Definition {} was dropped onto {}",
                  sourceDefinition.getNameAndUuidString(), sceneEntityDef.getNameAndUuidString());

        auto& parentDef = mDragDropSource.parentDefinition.value().get();

        sceneEntityDef.createChildDefinitionFrom(sourceDefinition);
        parentDef.removeChildDefinition(sourceDefinition);


        // Clear DragDrop pointers
        mDragDropSource.selfDefinition.reset();
        mDragDropSource.parentDefinition.reset();
      }
      ImGui::EndDragDropTarget();
    }

    // Show Node Contents
    if(nodeOpen)
    {
      for (auto& child : sceneEntityDef.getChildDefinitionsVector())
      {
        addEntityNode(child);
      }
      ImGui::TreePop();
    }
    ImGui::PopID();
  }


  void
  ProjectBrowser::removeNodeSelection
  (Definition& def)
  {
    auto itr = find_if(mSelectedNodes.begin(), mSelectedNodes.end(),
        [&](reference_wrapper<Definition>& next) {
        return next.get().getUuid() == def.getUuid(); });
    if (itr != mSelectedNodes.end()) mSelectedNodes.erase(itr);
  }
}

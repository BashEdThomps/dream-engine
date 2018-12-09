
#include "ProjectBrowser.h"
#include "PropertiesWindow.h"
#include "../../deps/ImGui/imguifilesystem.h"
#include <sstream>
#include "../../DTState.h"

#include "../../../DreamCore/Components/AssetDefinition.h"
#include "../../../DreamCore/Project/Project.h"
#include "../../../DreamCore/Project/ProjectDefinition.h"
#include "../../../DreamCore/Project/ProjectRuntime.h"
#include "../../../DreamCore/Scene/SceneDefinition.h"
#include "../../../DreamCore/Scene/SceneRuntime.h"
#include "../../../DreamCore/Scene/SceneObject/SceneObjectDefinition.h"
#include "../../../DreamCore/Scene/SceneObject/SceneObjectRuntime.h"

using std::stringstream;

namespace DreamTool
{
    ProjectBrowser::ProjectBrowser
    (
        DTState* project
    )
        : ImGuiWidget(project)
    {}

    ProjectBrowser::~ProjectBrowser
    ()
    {
        setLogClassName("ProjectBrowser");
    }

    void
    ProjectBrowser::draw
    ()
    {
        if (mState->project)
        {
            ImGui::Begin("Project Browser",&mVisible);
            drawProjectTree();
            ImGui::Separator();
            drawAssetTree();
            ImGui::End();
        }
    }

    void
    ProjectBrowser::drawProjectTree
    ()
    {
        auto log = getLog();
        // Project Tree
        auto projDef = mState->project->getDefinition();
        ImGui::Text("Scenegraph");
        ImGui::Separator();

        if (projDef == nullptr)
        {
            ImGui::BulletText("No Project Open");
            return;
        }

        ImGui::PushID("ProjectTree");
        int treeID = 0;
        if (ImGui::TreeNodeEx((void*)(intptr_t)++treeID,node_flags,projDef->getName().c_str(),0))
        {
            if (ImGui::IsItemClicked())
            {
                log->error("Project clicked {}", projDef->getName());
                mState->propertiesWindow.pushPropertyTarget
                (
                    Project,
                    projDef,
                    mState->project->getRuntime()
                );
            }

            int sdTreeID = 0;
            for (SceneDefinition* sDef : projDef->getSceneDefinitionsVector())
            {
                if (ImGui::TreeNodeEx((void*)(intptr_t)++sdTreeID,node_flags,sDef->getName().c_str(),0))
                {
                    if (ImGui::IsItemClicked())
                    {
                        log->error("Scene Clicked {}", sDef->getName());
                        auto pRunt = mState->project->getRuntime();
                        SceneRuntime* sRunt = nullptr;
                        if (pRunt)
                        {
                            sRunt = pRunt->getActiveSceneRuntime();
                        }

                        if (sRunt != nullptr)
                        {
                            if (sRunt->getUuid().compare(sDef->getUuid()) != 0)
                            {
                                log->error("Scene runtime != scene definition \n{} vs {}", sDef->getUuid(), sRunt->getUuid());
                                sRunt = nullptr;
                            }
                        }
                        mState->propertiesWindow.pushPropertyTarget(Scene, sDef,sRunt);
                    }

                    SceneObjectDefinition* rootSo = sDef->getRootSceneObjectDefinition();
                    addSceneObject(1,rootSo);
                    ImGui::TreePop();
                } // Scene Name
            }

            ImGui::TreePop();
        } // Project Name
        ImGui::PopID();
    }

    void
    ProjectBrowser::addSceneObject
    (int treeId, SceneObjectDefinition* def)
    {
        auto log = getLog();
        if (def != nullptr)
        {
            ImGuiTreeNodeFlags flags = (def->getChildCount() == 0 ? leaf_flags : node_flags);
            auto projRunt = mState->project->getRuntime();
            ImGui::PushID(treeId);
            if(ImGui::TreeNodeEx(
                   (void*)(intptr_t)treeId,
                   flags,
                   def->getName().c_str(),
                   0)
            )
            {
                SceneObjectRuntime* soRt = nullptr;
                if (projRunt->getActiveSceneRuntime())
                {
                    soRt = projRunt->getActiveSceneRuntime()->getSceneObjectRuntimeByUuid(def->getUuid());
                }

                if (ImGui::IsItemClicked())
                {
                    if (soRt)
                    {
                        mState->selectionHighlighter.setSelectedSceneObject(soRt);
                    }
                    log->error("SceneObject Clicked {}",def->getName());
                    mState->propertiesWindow.pushPropertyTarget(SceneObject, def, soRt);
                }

                // Our buttons are both drag sources and drag targets here!
                if (def->getParentSceneObject() != nullptr)
                {
                    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
                    {
                        ImGui::SetDragDropPayload(Constants::SCENE_OBJECT.c_str(), &def, sizeof(SceneObjectDefinition*));
                        ImGui::Text("Copy: %s",def->getName().c_str());
                        ImGui::EndDragDropSource();
                    }
                }

                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Constants::SCENE_OBJECT.c_str()))
                    {
                        IM_ASSERT(payload->DataSize == sizeof(SceneObjectDefinition*));
                        SceneObjectDefinition* droppedDef = *(static_cast<SceneObjectDefinition**>(payload->Data));
                        log->error(
                            "Definition {} was dropped onto {}",
                            droppedDef->getNameAndUuidString(),
                            def->getNameAndUuidString()
                        );
                        SceneObjectDefinition* newDef = new SceneObjectDefinition(def,def->getSceneDefinition(),droppedDef->getJson());
                        newDef->loadChildSceneObjectDefinitions(true);
                        def->addChildDefinition(newDef);
                        if (soRt)
                        {
                            soRt->createChildRuntime(newDef);
                        }
                    }
                    ImGui::EndDragDropTarget();
                }

                int childTreeId = 0;
                for (SceneObjectDefinition* child : def->getChildDefinitionsList())
                {
                    addSceneObject(++childTreeId, child);
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
    }

    void ProjectBrowser::drawAssetTree
    ()
    {
        auto log = getLog();
        auto projDef = mState->project->getDefinition();
        ImGui::Text("Assets");
        ImGui::Separator();

        if (projDef == nullptr)
        {
            return;
        }

        vector<string> assetTypes = Constants::DREAM_ASSET_TYPES_READABLE_VECTOR;
        int assetTypeIndex=-1;
        if(StringCombo("New",&assetTypeIndex,assetTypes,assetTypes.size()))
        {
            auto type = Constants::getAssetTypeEnumFromString(assetTypes.at(assetTypeIndex));
            auto newDef = projDef->createNewAssetDefinition(type);
            mState->propertiesWindow.pushPropertyTarget(Asset,newDef,nullptr);
            projDef->regroupAssetDefinitions();
        }

        ImGui::PushID("AssetTree");
        for (auto name : Constants::DREAM_ASSET_TYPES_READABLE_VECTOR)
        {
            AssetType type = Constants::getAssetTypeEnumFromString(name);
            auto assets = projDef->getAssetDefinitionsVector(type);
            auto typeGroups = projDef->getAssetDefinitionGroups()[type];
            stringstream nameCount;
            nameCount <<  name << " (" <<  assets.size() << ")";
            ImGui::PushID(name.c_str());
            static string selectedAssetType = "";
            ImGui::SetNextTreeNodeOpen(selectedAssetType.compare(name) == 0);
            if (ImGui::CollapsingHeader(nameCount.str().c_str(),node_flags))
            {
                selectedAssetType = name;
                int assetDefTreeId = 0;
                for (string group : typeGroups)
                {
                    ImGui::PushID(group.c_str());
                    if (ImGui::TreeNode(group.c_str()))
                    {
                        for (auto asset : assets)
                        {
                            if (asset->getGroup().compare(group) == 0)
                            {
                                if (ImGui::TreeNodeEx((void*)(intptr_t)++assetDefTreeId,leaf_flags,asset->getName().c_str(),0))
                                {
                                    if (ImGui::IsItemClicked())
                                    {
                                        log->error("Asset Definition Clicked {}", asset->getName());
                                        mState->propertiesWindow.pushPropertyTarget(Asset, asset, nullptr);
                                    }
                                    ImGui::TreePop();
                                }

                            }
                        }
                        ImGui::TreePop();
                    }
                    ImGui::PopID();
                }
            }
            ImGui::PopID();
        } // Asset Type Node
        ImGui::PopID();
    }
}


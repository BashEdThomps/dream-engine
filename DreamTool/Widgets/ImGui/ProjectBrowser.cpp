
#ifdef WIN32
#include <Windows.h>
#endif

#include "ProjectBrowser.h"
#include "PropertiesWindow.h"
#include "ImFileSystem/imguifilesystem.h"
#include <sstream>
#include "../../DTState.h"

#include "../../../DreamCore/Components/AssetDefinition.h"
#include "../../../DreamCore/Project/Project.h"
#include "../../../DreamCore/Project/ProjectDefinition.h"
#include "../../../DreamCore/Project/ProjectRuntime.h"
#include "../../../DreamCore/Scene/SceneDefinition.h"
#include "../../../DreamCore/Scene/SceneRuntime.h"
#include "../../../DreamCore/Scene/Actor/ActorDefinition.h"
#include "../../../DreamCore/Scene/Actor/ActorRuntime.h"

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
#ifdef DREAM_LOG
        setLogClassName("ProjectBrowser");
#endif
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

#ifdef DREAM_LOG
        auto log = getLog();
#endif
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
        bool projectNodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)++treeID,node_flags,projDef->getName().c_str(),0);

        if (ImGui::IsItemClicked())
        {

#ifdef DREAM_LOG
            log->trace("Project clicked {}", projDef->getName());
#endif
            mState->propertiesWindow.pushPropertyTarget
            (
                Project,
                projDef,
                mState->project->getRuntime()
            );
        }

        if (projectNodeOpen)
        {
            int sdTreeID = 0;
            for (SceneDefinition* sDef : projDef->getSceneDefinitionsVector())
            {
                bool sceneNodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)++sdTreeID,
                    node_flags, sDef->getName().c_str(), 0);

                if (ImGui::IsItemClicked())
                {
#ifdef DREAM_LOG
                    log->trace("Scene Clicked {}", sDef->getName());
#endif
                    auto pRunt = mState->project->getRuntime();
                    SceneRuntime* sRunt = nullptr;

                    if (pRunt)
                    {
                        sRunt = pRunt->getActiveSceneRuntime();
                    }

                    if (sRunt != nullptr)
                    {
                        if (sRunt->getUuid() != sDef->getUuid())
                        {
#ifdef DREAM_LOG
                            log->trace("Scene runtime != scene definition \n{} vs {}", sDef->getUuid(), sRunt->getUuid());
#endif
                            sRunt = nullptr;
                        }
                    }
                    mState->propertiesWindow.pushPropertyTarget(Scene, sDef,sRunt);
                }

                if (sceneNodeOpen)
                {
                    ActorDefinition* rootSo = sDef->getRootActorDefinition();
                    addActor(rootSo);
                    ImGui::TreePop();
                }
            }

            ImGui::TreePop();
        } // Project Name
        ImGui::PopID();
    }

    void
    ProjectBrowser::addActor
    (ActorDefinition* def)
    {
        int treeId = 0;
#ifdef DREAM_LOG
        auto log = getLog();
#endif

        if (def != nullptr)
        {
            auto projRunt = mState->project->getRuntime();
            SceneRuntime* sRunt = projRunt->getActiveSceneRuntime();
            ActorRuntime* soRunt = nullptr;

            if (sRunt)
            {
                soRunt = sRunt->getActorRuntimeByUuid(def->getUuid());
            }

            ImGuiTreeNodeFlags flags = (def->getChildCount() == 0 ? leaf_flags : node_flags);
            ImGui::PushID(def->getUuid());

            bool isSelected = find(mSelectedNodes.begin(), mSelectedNodes.end(), def) != mSelectedNodes.end();

            // Flags
            stringstream nameStr;
            stringstream flagsStr;
            if (def->getIsTemplate())
            {
               flagsStr << "T";
            }
            if (def->getDeferred() > 0.0)
            {
                flagsStr << "D";
            }
            if (def->getDieAfter() > 0.0)
            {
                flagsStr << "X";
            }

            if (flagsStr.str().size() > 0)
            {
                nameStr << "(";
                nameStr << flagsStr.str();
                nameStr << ") ";
            }

            nameStr << def->getName();
            bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)treeId,
               flags | (isSelected ? ImGuiTreeNodeFlags_Selected : 0),
               nameStr.str().c_str(), 0);

            // Actor Context Menu
            bool deleteClicked = false;
            bool copyToClicked = false;

            if (ImGui::BeginPopupContextItem())
            {
                const char* defName = def->getName().c_str();
                if (mSelectedNodes.size() > 1)
                {
                    ImGui::Text("%d objects selected",static_cast<int>(mSelectedNodes.size()));
                }
                else
                {
                    ImGui::Text("%s",defName);
                }

                // No Root Deletion
                if (def->getParentActor())
                {
                    // Deletion
                    ImGui::Separator();
                    char deleteBuffer[buf_sz];
                    snprintf(deleteBuffer, buf_sz, "Delete %s",defName);
                    deleteClicked = ImGui::MenuItem(deleteBuffer);
                }

                // Copying
                ImGui::Separator();

                char copySelectedBuffer[buf_sz];
                snprintf(copySelectedBuffer,buf_sz,"Copy selected object(s) into %s",defName);
                copyToClicked = ImGui::MenuItem(copySelectedBuffer);

                ImGui::EndPopup();
            }

            // Context Menu Items
            if (deleteClicked)
            {
                auto parent = def->getParentActor();
                if (parent)
                {
                    parent->removeChildDefinition(def);
                }
                if (soRunt)
                {
                    auto parent = soRunt->getParentRuntime();
                    parent->removeChildRuntime(soRunt);
                }
                mState->selectionHighlighter.clearSelection();
                mState->propertiesWindow.removeFromHistory(def);
                mState->propertiesWindow.popPropertyTarget();
                mSelectedNodes.clear();
            }
            else if (copyToClicked)
            {
                for (auto node : mSelectedNodes)
                {
                    auto defToCreate = dynamic_cast<ActorDefinition*>(node);
                    ActorDefinition* newDef = new ActorDefinition(def,def->getSceneDefinition(),defToCreate->getJson(),true);
                    newDef->loadChildActorDefinitions(true);
                    def->addChildDefinition(newDef);
                    if (soRunt)
                    {
                        soRunt->createAndAddChildRuntime(newDef);
                    }
                }
                mSelectedNodes.clear();
            }

            // Node Selection
            if (ImGui::IsItemClicked())
            {
                if (ImGui::GetIO().KeyCtrl)
                {
                    // CTRL+click to toggle
                    mSelectedNodes.push_back(def);
                }
                else
                {
                    // Click to single-select or CTRL+click to toggle
                    mSelectedNodes.clear();
                    mSelectedNodes.push_back(def);
                }

                if (soRunt)
                {
                    mState->selectionHighlighter.setSelectedActor(soRunt);
                }
#ifdef DREAM_LOG
                log->trace("Actor Clicked {}",def->getName());
#endif
                mState->propertiesWindow.pushPropertyTarget(Actor, def, soRunt);
            }

            // Drag Source
            if (def->getParentActor() && ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
            {
                mDragDropSource.objectDef = def;
                mDragDropSource.parentDef = def->getParentActor();

                ImGui::SetDragDropPayload( Constants::ACTOR.c_str(),
                    &mDragDropSource, sizeof(ActorDragSource*));
                ImGui::Text("Reparent %s",def->getName().c_str());
                ImGui::EndDragDropSource();
            }

            // Drop Target
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Constants::ACTOR.c_str()))
                {
                    IM_ASSERT(payload->DataSize == sizeof(ActorDragSource*));
#ifdef DREAM_LOG
                    log->trace
                    (
                        "Definition {} was dropped onto {}",
                        mDragDropSource.objectDef->getNameAndUuidString(),
                        def->getNameAndUuidString()
                    );
#endif

                    mDragDropSource.parentDef->removeChildDefinition(mDragDropSource.objectDef,false);
                    def->adoptChildDefinition(mDragDropSource.objectDef);

                    if (soRunt)
                    {
                        soRunt->createAndAddChildRuntime(mDragDropSource.objectDef);

                        // get old parent runtime and remove children that were reparented
                        auto oldParent = sRunt->getActorRuntimeByUuid(mDragDropSource.parentDef->getUuid());
                        if (oldParent)
                        {
                            auto oldRuntime = oldParent->getChildRuntimeByUuid(mDragDropSource.objectDef->getUuid());
                            if (oldRuntime)
                            {
                                oldParent->removeChildRuntime(oldRuntime);
                            }
                        }
                        // Clear from properties
                        mState->propertiesWindow.removeFromHistory(mDragDropSource.objectDef);
                        mState->propertiesWindow.popPropertyTarget();
                    }
                    // Clear DragDrop pointers
                    mDragDropSource.objectDef = nullptr;
                    mDragDropSource.parentDef = nullptr;
                }
                ImGui::EndDragDropTarget();
            }

            // Show Node Contents
            if(nodeOpen)
            {
                for (ActorDefinition* child : def->getChildDefinitionsList())
                {
                    addActor(child);
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
    }

    void
    ProjectBrowser::drawAssetTree
    ()
    {
#ifdef DREAM_LOG
        auto log = getLog();
#endif
        auto projDef = mState->project->getDefinition();
        ImGui::Text("Assets");
        ImGui::Separator();

        if (projDef == nullptr)
        {
            return;
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
            bool headerOpen = ImGui::CollapsingHeader(nameCount.str().c_str(),node_flags);

            // Context Menu
            if (ImGui::BeginPopupContextItem())
            {
                char buf[buf_sz];
                snprintf(buf,buf_sz,"New %s",name.c_str());
                bool newClicked = ImGui::MenuItem(buf);
                if (newClicked)
                {
                    auto newDef = projDef->createNewAssetDefinition(type);
                    mState->propertiesWindow.pushPropertyTarget(Asset,newDef,nullptr);
                    projDef->regroupAssetDefinitions();
                }
                ImGui::EndPopup();
            }

            if (headerOpen)
            {
                // Group Nodes
                selectedAssetType = name;
                int assetDefTreeId = 0;
                for (string group : typeGroups)
                {
                    ImGui::PushID(group.c_str());
                    if (ImGui::TreeNode(group.c_str()))
                    {
                        // Asset Nodes
                        for (auto asset : assets)
                        {
                            if (asset->getGroup().compare(group) == 0)
                            {
                                bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)++assetDefTreeId,leaf_flags,asset->getName().c_str(),0);

                                // Asset Def Contex Menu
                                if (ImGui::BeginPopupContextItem())
                                {
                                    char buf[buf_sz];
                                    snprintf(buf,buf_sz,"Delete %s",asset->getName().c_str());
                                    bool deleteClicked = ImGui::MenuItem(buf);
                                    if (deleteClicked)
                                    {
                                    }
                                    ImGui::EndPopup();
                                }

                                if (nodeOpen)
                                {
                                    if (ImGui::IsItemClicked())
                                    {
#ifdef DREAM_LOG
                                        log->error("Asset Definition Clicked {}", asset->getName());
#endif
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


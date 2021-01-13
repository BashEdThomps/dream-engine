#include "ProjectBrowser.h"

#ifdef WIN32
#include <Windows.h>
#endif

#include "PropertiesWindow.h"
#include "DTContext.h"

#include <DreamCore.h>
#include <ImFileSystem.h>
#include <sstream>

using std::stringstream;

namespace DreamTool
{
    ProjectBrowser::ProjectBrowser(DTContext* project)
        : ImGuiWidget(project)
    {}

    ProjectBrowser::~ProjectBrowser
    ()
    {
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

            LOG_TRACE("Project clicked {}", projDef->getName());
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
                    LOG_TRACE("Scene Clicked {}", sDef->getName());
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
                            LOG_TRACE("Scene runtime != scene definition \n{} vs {}", sDef->getUuid(), sRunt->getUuid());
                            sRunt = nullptr;
                        }
                    }
                    mState->propertiesWindow.pushPropertyTarget(Scene, sDef,sRunt);
                }

                if (sceneNodeOpen)
                {
                    EntityDefinition* rootSo = sDef->getRootEntityDefinition();
                    addEntity(rootSo);
                    ImGui::TreePop();
                }
            }

            ImGui::TreePop();
        } // Project Name
        ImGui::PopID();
    }

    void
    ProjectBrowser::addEntity
    (EntityDefinition* def)
    {
        int treeId = 0;

        if (def != nullptr)
        {
            auto projRunt = mState->project->getRuntime();
            SceneRuntime* sRunt = projRunt->getActiveSceneRuntime();
            EntityRuntime* soRunt = nullptr;

            if (sRunt)
            {
                soRunt = sRunt->getEntityRuntimeByUuid(def->getUuid());
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

            // Entity Context Menu
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
                if (def->getParentEntity())
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
                auto parent = def->getParentEntity();
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
                    auto defToCreate = dynamic_cast<EntityDefinition*>(node);
                    EntityDefinition* newDef = new EntityDefinition(def,def->getSceneDefinition(),defToCreate->getJson(),true);
                    newDef->loadChildEntityDefinitions(true);
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
                    mState->selectionHighlighter.setSelectedEntity(soRunt);
                }
                LOG_TRACE("Entity Clicked {}",def->getName());
                mState->propertiesWindow.pushPropertyTarget(Entity, def, soRunt);
            }

            // Drag Source
            if (def->getParentEntity() && ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
            {
                mDragDropSource.objectDef = def;
                mDragDropSource.parentDef = def->getParentEntity();

                ImGui::SetDragDropPayload( Constants::ENTITY.c_str(),
                    &mDragDropSource, sizeof(EntityDragSource*));
                ImGui::Text("Reparent %s",def->getName().c_str());
                ImGui::EndDragDropSource();
            }

            // Drop Target
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Constants::ENTITY.c_str()))
                {
                    IM_ASSERT(payload->DataSize == sizeof(EntityDragSource*));
                    LOG_TRACE
                    (
                        "Definition {} was dropped onto {}",
                        mDragDropSource.objectDef->getNameAndUuidString(),
                        def->getNameAndUuidString()
                    );

                    mDragDropSource.parentDef->removeChildDefinition(mDragDropSource.objectDef,false);
                    def->adoptChildDefinition(mDragDropSource.objectDef);

                    if (soRunt)
                    {
                        soRunt->createAndAddChildRuntime(mDragDropSource.objectDef);

                        // get old parent runtime and remove children that were reparented
                        auto oldParent = sRunt->getEntityRuntimeByUuid(mDragDropSource.parentDef->getUuid());
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
                for (EntityDefinition* child : def->getChildDefinitionsList())
                {
                    addEntity(child);
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
                                        LOG_ERROR("Asset Definition Clicked {}", asset->getName());
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


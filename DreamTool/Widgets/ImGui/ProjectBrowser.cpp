#include "ProjectBrowser.h"

#ifdef WIN32
#include <Windows.h>
#endif

#include "PropertiesWindow.h"
#include "DreamToolContext.h"

#include <DreamCore.h>
#include <nfd.h>
#include <sstream>

using std::stringstream;

namespace octronic::dream::tool
{
    ProjectBrowser::ProjectBrowser(DreamToolContext* project)
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
        Project* project = mContext->getProject();

        if (project)
        {
            ImGui::Begin("Project",&mVisible);
            // Project Tree
            ProjectDefinition* projDef = project->getDefinition();

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

                LOG_TRACE("ProjectBrowser: Project clicked {}", projDef->getName());
                mContext->getPropertiesWindow()->pushPropertyTarget
                        (
                            PropertyType_Project,
                            projDef,
                            project->getRuntime()
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
                        LOG_TRACE("ProjectBrowser: Scene Clicked {}", sDef->getName());
                        ProjectRuntime* pRunt = project->getRuntime();
                        SceneRuntime* sRunt = nullptr;

                        if (pRunt)
                        {
                            sRunt = pRunt->getActiveSceneRuntime();
                        }

                        if (sRunt != nullptr)
                        {
                            if (sRunt->getUuid() != sDef->getUuid())
                            {
                                LOG_TRACE("ProjectBrowser: Scene runtime != scene definition \n{} vs {}", sDef->getUuid(), sRunt->getUuid());
                                sRunt = nullptr;
                            }
                        }
                        mContext->getPropertiesWindow()->pushPropertyTarget(PropertyType_Scene, sDef,sRunt);
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

            ImGui::End();
        }
    	else
        {
            ImGui::Text("No Project Open");
        }
    }

    void
    ProjectBrowser::addEntity
    (EntityDefinition* def)
    {
        Project* project = mContext->getProject();
        int treeId = 0;

        if (def != nullptr)
        {
            ProjectRuntime* projectRuntime = project->getRuntime();
            SceneRuntime* sceneRuntime = projectRuntime->getActiveSceneRuntime();
            EntityRuntime* entityRuntime = nullptr;

            if (sceneRuntime)
            {
                entityRuntime = sceneRuntime->getEntityRuntimeByUuid(def->getUuid());
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

            if (sceneRuntime)
            {
                CameraRuntime* cam = sceneRuntime->getCamera();
                if (cam->getCameraEntityRuntime() == entityRuntime)
                {
					if (!flagsStr.str().empty())
					{
						flagsStr << ",";
					}
					flagsStr << "Cam";
                }
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
                string defName = def->getName();
                if (mSelectedNodes.size() > 1)
                {
                    ImGui::Text("%d objects selected",static_cast<int>(mSelectedNodes.size()));
                }
                else
                {
                    ImGui::Text("%s",defName.c_str());
                }

                // No Root Deletion
                if (def->getParentEntity())
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
                auto parent = def->getParentEntity();
                if (parent)
                {
                    parent->removeChildDefinition(def);
                }
                if (entityRuntime)
                {
                    auto parent = entityRuntime->getParentEntityRuntime();
                    parent->removeChildRuntime(entityRuntime);
                }
                mContext->getSelectionHighlighter()->clearSelection();
                mContext->getPropertiesWindow()->removeFromHistory(def);
                mContext->getPropertiesWindow()->popPropertyTarget();
                mSelectedNodes.clear();
            }
            else if (copyToClicked)
            {
                for (auto node : mSelectedNodes)
                {
                    auto defToCreate = dynamic_cast<EntityDefinition*>(node);
                    EntityDefinition* newDef = new EntityDefinition(def,def->getSceneDefinition(),defToCreate->toJson(),true);
                    newDef->loadChildEntityDefinitions(true);
                    def->addChildDefinition(newDef);
                    if (entityRuntime)
                    {
                        entityRuntime->createAndAddChildRuntime(newDef);
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

                if (entityRuntime)
                {
                    mContext->getSelectionHighlighter()->setSelectedEntity(entityRuntime);
                }
                LOG_TRACE("ProjectBrowser: Entity Clicked {}",def->getName());
                mContext->getPropertiesWindow()->pushPropertyTarget(PropertyType_Entity, def, entityRuntime);
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
                                "ProjectBrowser: Definition {} was dropped onto {}",
                                mDragDropSource.objectDef->getNameAndUuidString(),
                                def->getNameAndUuidString()
                                );

                    mDragDropSource.parentDef->removeChildDefinition(mDragDropSource.objectDef,false);
                    def->adoptChildDefinition(mDragDropSource.objectDef);

                    if (entityRuntime)
                    {
                        entityRuntime->createAndAddChildRuntime(mDragDropSource.objectDef);

                        // get old parent runtime and remove children that were reparented
                        auto oldParent = sceneRuntime->getEntityRuntimeByUuid(mDragDropSource.parentDef->getUuid());
                        if (oldParent)
                        {
                            auto oldRuntime = oldParent->getChildRuntimeByUuid(mDragDropSource.objectDef->getUuid());
                            if (oldRuntime)
                            {
                                oldParent->removeChildRuntime(oldRuntime);
                            }
                        }
                        // Clear from properties
                        mContext->getPropertiesWindow()->removeFromHistory(mDragDropSource.objectDef);
                        mContext->getPropertiesWindow()->popPropertyTarget();
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
                for (EntityDefinition* child : def->getChildDefinitionsVector())
                {
                    addEntity(child);
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
    }
}

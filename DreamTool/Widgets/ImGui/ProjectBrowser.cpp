#include "ProjectBrowser.h"
#include "PropertiesWindow.h"
#include "../../deps/ImGui/imguifilesystem.h"
#include <sstream>

using std::stringstream;

namespace DreamTool
{
    ProjectBrowser::ProjectBrowser
    (Project* project, PropertiesWindow* properties)
        : ImGuiWidget(project),
          mPropertiesWindowHandle(properties)
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
        ImGui::Begin("Project Browser");
        drawProjectTree();
        ImGui::Separator();
        drawAssetTree();
        ImGui::End();
    }

    void
    ProjectBrowser::drawProjectTree
    ()
    {
        auto log = getLog();
        // Project Tree
        auto projDef = mProject->getProjectDefinition();
        ImGui::Text("Scenegraph");
        ImGui::Separator();

        if (projDef == nullptr)
        {
            ImGui::BulletText("No Project Open");
            return;
        }

        int treeID = 0;
        if (ImGui::TreeNodeEx((void*)(intptr_t)++treeID,node_flags,projDef->getName().c_str(),0))
        {
            if (ImGui::IsItemClicked())
            {
                log->error("Project clicked {}", projDef->getName());
                mPropertiesWindowHandle->pushPropertyTarget
                (
                    PROP_TYPE_PROJECT,
                    projDef,
                    mProject->getProjectRuntime()
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
                        auto pRunt = mProject->getProjectRuntime();
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
                        mPropertiesWindowHandle->pushPropertyTarget(PROP_TYPE_SCENE, sDef,sRunt);
                    }

                    SceneObjectDefinition* rootSo = sDef->getRootSceneObjectDefinition();
                    addSceneObject(1,rootSo);
                    ImGui::TreePop();
                } // Scene Name
            }

            ImGui::TreePop();
        } // Project Name
    }

    void
    ProjectBrowser::addSceneObject
    (int treeId, SceneObjectDefinition* def)
    {
        auto log = getLog();
        if (def != nullptr)
        {
            ImGuiTreeNodeFlags flags = (def->getChildCount() == 0 ? leaf_flags : node_flags);
            auto projRunt = mProject->getProjectRuntime();
            if(ImGui::TreeNodeEx(
                   (void*)(intptr_t)treeId,
                   flags,
                   def->getName().c_str(),
                   0)
            )
            {
                if (ImGui::IsItemClicked())
                {
                    SceneObjectRuntime* soRt = nullptr;
                    if (projRunt->getActiveSceneRuntime())
                    {
                        soRt = projRunt->getActiveSceneRuntime()->getSceneObjectRuntimeByUuid(def->getUuid());
                    }
                    log->error("SceneObject Clicked {}",def->getName());
                    mPropertiesWindowHandle->pushPropertyTarget(PROP_TYPE_SCENE_OBJECT, def, soRt);
                }

                int childTreeId = 0;
                for (SceneObjectDefinition* child : def->getChildDefinitionsList())
                {
                    addSceneObject(++childTreeId, child);
                }


                ImGui::TreePop();
            }


        }
    }

    void ProjectBrowser::drawAssetTree
    ()
    {
        auto log = getLog();
        auto projDef = mProject->getProjectDefinition();
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
            projDef->createNewAssetDefinition(type);
        }

        int assetTypeTreeId = 0;
        for (auto name : Constants::DREAM_ASSET_TYPES_READABLE_VECTOR)
        {
            AssetType type = Constants::getAssetTypeEnumFromString(name);
            auto assets = projDef->getAssetDefinitionsVector(type);
            stringstream nameCount;
            nameCount <<  name << " (" <<  assets.size() << ")";
            if (ImGui::TreeNodeEx((void*)(intptr_t)++assetTypeTreeId,node_flags,nameCount.str().c_str(),0))
            {
                int assetDefTreeId = 0;
                for (auto asset : assets)
                {
                    if (ImGui::TreeNodeEx((void*)(intptr_t)++assetDefTreeId,leaf_flags,asset->getName().c_str(),0))
                    {
                        if (ImGui::IsItemClicked())
                        {
                            log->error("Asset Definition Clicked {}", asset->getName());
                            mPropertiesWindowHandle->pushPropertyTarget(PROP_TYPE_ASSET, asset, nullptr);
                        }
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
        } // Asset Type Node
    }
}


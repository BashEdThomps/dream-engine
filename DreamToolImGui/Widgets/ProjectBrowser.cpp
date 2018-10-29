#include "ProjectBrowser.h"
#include "PropertiesWindow.h"
#include "../deps/ImGui/imguifilesystem.h"
#include "ImGuiHelpers.h"
#include <sstream>

using Dream::SceneDefinition;
using Dream::SceneRuntime;
using Dream::SceneObjectDefinition;
using Dream::IAssetDefinition;
using Dream::Constants;
using Dream::IAssetDefinition;
using Dream::ProjectRuntime;
using Dream::SceneObjectRuntime;
using Dream::AssetType;

using std::stringstream;

namespace DreamTool
{
    ProjectBrowser::ProjectBrowser
    (Project* project, PropertiesWindow* properties)
        : DTWidget(project),
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
        mTreeID = 0;
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
        static ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;

       // Project Tree
        auto projDef = mProject->getProjectDefinition();
        auto projRunt = mProject->getProjectRuntime();

        ImGui::Text("Scenegraph");

        ImGui::Separator();
        if (projDef == nullptr)
        {
            ImGui::BulletText("No Project Open");
            return;
        }


        if (ImGui::TreeNodeEx((void*)(intptr_t)mTreeID++,node_flags,projDef->getName().c_str(),0))
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

            for (SceneDefinition* sDef : projDef->getSceneDefinitionsVector())
            {
                if (ImGui::TreeNodeEx((void*)(intptr_t)mTreeID++,node_flags,sDef->getName().c_str(),0))
                {
                    if (ImGui::IsItemClicked())
                    {
                        log->error("Scene Clicked {}", sDef->getName());

                        SceneRuntime* sRunt = nullptr;// TODO projRunt->getActiveSceneRuntime();
                        if (sRunt != nullptr)
                        {
                            if (sRunt->getUuid().compare(sDef->getUuid()) != 0)
                            {
                                sRunt = nullptr;
                                log->error("Scene runtime != scene definition \n{} vs {}", sDef->getUuid(), sRunt->getUuid());
                            }
                        }

                        mPropertiesWindowHandle->pushPropertyTarget
                        (
                            PROP_TYPE_SCENE,
                            sDef,
                            sRunt
                        );
                    }
                    SceneObjectDefinition* rootSo = sDef->getRootSceneObjectDefinition();
                    addSceneObject(rootSo);
                    ImGui::TreePop();
                } // Scene Name
            }
            ImGui::TreePop();
        } // Project Name
    }

    void
    ProjectBrowser::addSceneObject
    (SceneObjectDefinition* def)
    {
        auto log = getLog();
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
        if (def != nullptr)
        {
            auto projDef = mProject->getProjectDefinition();
            auto projRunt = mProject->getProjectRuntime();

            if (def->getChildCount() == 0)
            {
                node_flags |= ImGuiTreeNodeFlags_Leaf;
            }
            if (ImGui::TreeNodeEx((void*)(intptr_t)mTreeID++,node_flags,def->getName().c_str(),0))
            {
                if (ImGui::IsItemClicked())
                {
                    SceneObjectRuntime* soRt = nullptr;
                    if (ProjectRuntime::CurrentSceneRuntime)
                    {
                        soRt = mProject
                            ->getProjectRuntime()
                            ->getSceneObjectRuntimeByUuid(
                                ProjectRuntime::CurrentSceneRuntime,
                                def->getUuid()
                        );
                    }
                    log->error("SceneObject Clicked {}",def->getName());
                    mPropertiesWindowHandle->pushPropertyTarget
                    (
                        PROP_TYPE_SCENE_OBJECT,
                        def,
                        soRt
                    );
                }
                for (SceneObjectDefinition* child : def->getChildDefinitionsList())
                {
                    addSceneObject(child);
                }
                ImGui::TreePop();
            } // Root SO
        }

    }

    void ProjectBrowser::drawAssetTree
    ()
    {
        auto log = getLog();
        static ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
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

        for (auto name : Constants::DREAM_ASSET_TYPES_READABLE_VECTOR)
        {
           AssetType type = Constants::getAssetTypeEnumFromString(name);

            auto assets = projDef->getAssetDefinitionsVector(type);
            stringstream nameCount;
            nameCount <<  name << " (" <<  assets.size() << ")";
           if (ImGui::TreeNodeEx((void*)(intptr_t)mTreeID++,node_flags,nameCount.str().c_str(),0))
           {
               for (auto asset : assets)
               {
                   if (ImGui::TreeNodeEx
                        (
                            (void*)(intptr_t)mTreeID++,
                            node_flags | ImGuiTreeNodeFlags_Leaf,
                            asset->getName().c_str(),
                            0
                        )
                    ){
                       if (ImGui::IsItemClicked())
                        {
                            log->error("Asset Definition Clicked {}", asset->getName());
                            mPropertiesWindowHandle->pushPropertyTarget(
                                PROP_TYPE_ASSET,
                                asset,
                                nullptr
                            );
                        }
                       ImGui::TreePop();
                   }
               }
               ImGui::TreePop();
           }
        } // Asset Type Node
    }
}


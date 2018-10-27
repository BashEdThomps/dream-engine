#include "ProjectBrowser.h"
#include "PropertiesWindow.h"

using Dream::SceneDefinition;
using Dream::SceneObjectDefinition;
using Dream::IAssetDefinition;
using Dream::Constants;
using Dream::IAssetDefinition;
using Dream::AssetType;

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
        auto log = getLog();
        static ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
        ImGui::Begin("Project Browser");
        auto projDef = mProject->getProjectDefinition();
        auto projRunt = mProject->getProjectRuntime();

        if (ImGui::TreeNodeEx((void*)(intptr_t)0,node_flags,projDef->getName().c_str(),0))
        {
            if (ImGui::IsItemClicked())
            {
                log->error("Project clicked {}", projDef->getName());
                mPropertiesWindowHandle->setPropertyType(PROP_TYPE_PROJECT);
                mPropertiesWindowHandle->setDefinition(projDef);
                mPropertiesWindowHandle->setRuntime(mProject->getProjectRuntime());
            }

            for (SceneDefinition* sDef : projDef->getSceneDefinitionsVector())
            {
                if (ImGui::TreeNodeEx((void*)(intptr_t)0,node_flags,sDef->getName().c_str(),0))
                {
                    if (ImGui::IsItemClicked())
                    {
                        log->error("Scene Clicked {}", sDef->getName());
                        mPropertiesWindowHandle->setPropertyType(PROP_TYPE_SCENE);
                        mPropertiesWindowHandle->setDefinition(sDef);
                        auto sRunt = projRunt->getActiveSceneRuntime();

                        if (sRunt->getUuid().compare(sDef->getUuid()) == 0)
                        {
                            mPropertiesWindowHandle->setRuntime(sRunt);
                        }
                        else
                        {
                            log->error("Scene runtime != scene definition \n{} vs {}", sDef->getUuid(), sRunt->getUuid());
                        }
                    }
                    SceneObjectDefinition* rootSo = sDef->getRootSceneObjectDefinition();
                    addSceneObject(rootSo);
                    ImGui::TreePop();
                } // Scene Name
            }
            ImGui::TreePop();
        } // Project Name
        ImGui::End();
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

            if (ImGui::TreeNodeEx((void*)(intptr_t)0,node_flags,def->getName().c_str(),0))
            {
                if (ImGui::IsItemClicked())
                {
                    log->error("SceneObject Clicked {}",def->getName());
                    mPropertiesWindowHandle->setPropertyType(PROP_TYPE_SCENE_OBJECT);
                    mPropertiesWindowHandle->setDefinition(def);
                    mPropertiesWindowHandle->setRuntime(projRunt ->getSceneObjectRuntimeByUuid(def->getUuid()));
                }
                for (SceneObjectDefinition* child : def->getChildDefinitionsList())
                {
                    addSceneObject(child);
                }
                ImGui::TreePop();
            } // Root SO
        }

    }
}


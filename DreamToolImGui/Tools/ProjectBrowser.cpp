#include "ProjectBrowser.h"

using Dream::SceneDefinition;
using Dream::SceneObjectDefinition;
using Dream::IAssetDefinition;
using Dream::Constants;
using Dream::IAssetDefinition;
using Dream::AssetType;

namespace DreamTool
{
    ProjectBrowser::ProjectBrowser
    (Project* project)
        : DTWidget(project)
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
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Begin("Project Browser");
        auto projDef = mProject->getProjectDefinition();

        if (ImGui::TreeNode(projDef->getName().c_str()))
        {
            for (SceneDefinition* sDef : projDef->getSceneDefinitionsList())
            {
                if (ImGui::TreeNode(sDef->getName().c_str()))
                {
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
        if (def != nullptr)
        {
            if (ImGui::TreeNode(def->getName().c_str()))
            {
                for (SceneObjectDefinition* child : def->getChildDefinitionsList())
                {
                    addSceneObject(child);
                }
                ImGui::TreePop();
            } // Root SO
        }

    }
}


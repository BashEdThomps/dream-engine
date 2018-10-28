#include "ProjectBrowser.h"
#include "PropertiesWindow.h"
#include "../deps/ImGui/imguifilesystem.h"

using Dream::SceneDefinition;
using Dream::SceneRuntime;
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
        mTreeID = 0;
        static ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
        ImGui::Begin("Project Browser");

        // New/Open/Save

        const bool newButtonClicked = ImGui::Button("New");
        {
            static ImGuiFs::Dialog newDlg;
            newDlg.chooseFolderDialog(newButtonClicked);
            if (strlen(newDlg.getChosenPath())>0)
            {
                ImGui::Text("Chosen file: \"%s\"",newDlg.getChosenPath());
            }
        }
        ImGui::SameLine();

        bool openButtonClicked = ImGui::Button("Open");
        {
            static ImGuiFs::Dialog openDlg;
            const char* chosenPath = openDlg.chooseFolderDialog(openButtonClicked);
            if (strlen(chosenPath) > 0)
            {
                auto projectDir = openDlg.getChosenPath();
                log->error("Opening project {}",projectDir);
                if(mProject->openFromDirectory(projectDir))
                {
                    mProject->createProjectRuntime();
                }
            }
        }
        ImGui::SameLine();

        if(ImGui::Button("Save"))
        {

        }

        ImGui::Separator();

       // Project Tree
        auto projDef = mProject->getProjectDefinition();
        auto projRunt = mProject->getProjectRuntime();

        if (projDef == nullptr)
        {
            ImGui::End();
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

            if (def->getChildCount() == 0)
            {
                node_flags |= ImGuiTreeNodeFlags_Leaf;
            }
            if (ImGui::TreeNodeEx((void*)(intptr_t)mTreeID++,node_flags,def->getName().c_str(),0))
            {
                if (ImGui::IsItemClicked())
                {
                    log->error("SceneObject Clicked {}",def->getName());
                    mPropertiesWindowHandle->pushPropertyTarget
                    (
                        PROP_TYPE_SCENE_OBJECT,
                        def,
                        nullptr
                        //projRunt->getSceneObjectRuntimeByUuid(sRunt, def->getUuid())
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
}


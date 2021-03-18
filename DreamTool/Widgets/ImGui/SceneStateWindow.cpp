#include "SceneStateWindow.h"
#include "DreamToolContext.h"
#include <DreamCore.h>

using octronic::dream::Project;
using octronic::dream::ProjectDefinition;
using octronic::dream::ProjectRuntime;

namespace octronic::dream::tool
{
    SceneStateWindow::SceneStateWindow(DreamToolContext* p)
        : ImGuiWidget(p,false)
    {
    }

    SceneStateWindow::~SceneStateWindow()
    {

    }

    void SceneStateWindow::draw()
    {
        auto project = mContext->getProject();
        if (project)
        {
            ImGui::Begin("Scene States",&mVisible);
            ImGui::Columns(3);
            ImGui::Text("Scene");
            ImGui::NextColumn();
            ImGui::Text("State");
            ImGui::NextColumn();
            ImGui::Text("Time");
            ImGui::NextColumn();


            auto pDef = project->getDefinition();
            auto pRunt = project->getRuntime();

            if (pDef)
            {
                for (auto sceneDef : pDef->getSceneDefinitionsVector())
                {
                    ImGui::Text("%s",sceneDef->getName().c_str());
                    ImGui::NextColumn();
                    if (pRunt)
                    {
                        auto srt = pRunt->getSceneRuntimeByUuid(sceneDef->getUuid());

                        string state;
                        unsigned long time = 0;
                        if (srt == nullptr)
                        {
                            state = "Not Loaded";
                        }
                        else
                        {
                            switch (srt->getState())
                            {
                                case SceneState::SCENE_STATE_TO_LOAD:
                                    state = "To Load";
                                    break;
                                case SceneState::SCENE_STATE_LOADED:
                                    state = "Loaded";
                                    break;
                                case SceneState::SCENE_STATE_ACTIVE:
                                    state = "Active";
                                    break;
                                case SceneState::SCENE_STATE_TO_DESTROY:
                                    state = "To Destroy";
                                    break;
                                case SceneState::SCENE_STATE_DESTROYED:
                                    state = "Destroyed";
                                    break;
                            }
                            time = srt->getSceneCurrentTime();
                        }
                        ImGui::Text("%s",state.c_str());
                        ImGui::NextColumn();
                        ImGui::Text("%lu",time);
                    }
                    else
                    {
                        ImGui::Text("Not Loaded");
                        ImGui::NextColumn();
                        ImGui::Text("---");
                    }
                }
            }

            ImGui::Columns(1);
            ImGui::End();
        }
    }
}

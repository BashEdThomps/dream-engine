#include "SceneStateWindow.h"
#include "../../DTState.h"
#include "../../../DreamCore/Project/Project.h"
#include "../../../DreamCore/Project/ProjectDefinition.h"
#include "../../../DreamCore/Project/ProjectRuntime.h"
#include "../../../DreamCore/Scene/SceneDefinition.h"
#include "../../../DreamCore/Scene/SceneRuntime.h"

namespace DreamTool
{
    SceneStateWindow::SceneStateWindow
    (DTState* p)
        : ImGuiWidget(p,false)
    {
        setLogClassName("SceneStateWindow");
    }

    SceneStateWindow::~SceneStateWindow
    ()
    {

    }

    void
    SceneStateWindow::draw
    ()
    {
        if (!mState->project)
        {

            ImGui::Begin("Scene States",&mVisible);
            ImGui::Columns(2);
            ImGui::Text("Scene");
            ImGui::NextColumn();
            ImGui::Text("State");
            ImGui::NextColumn();

            auto pDef = mState->project->getDefinition();
            auto pRunt = mState->project->getRuntime();

            if (pDef && pDef)
            {
                for (auto sceneDef : pDef->getSceneDefinitionsVector())
                {
                    ImGui::Text("%s",sceneDef->getName().c_str());
                    ImGui::NextColumn();
                    if (pRunt)
                    {
                        auto srt = pRunt->getSceneRuntimeByUuid(sceneDef->getUuid());

                        string state;
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
                        }
                        ImGui::Text("%s",state.c_str());
                    }
                    else
                    {
                        ImGui::Text("Not Loaded");
                    }
                    ImGui::NextColumn();
                }
            }

            ImGui::Columns(1);
            ImGui::End();
        }
    }
}

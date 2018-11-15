#include "SceneStateWindow.h"
#include "../../DTState.h"

namespace DreamTool
{
    SceneStateWindow::SceneStateWindow
    (DTState* p)
        : ImGuiWidget(p)
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
        ImGui::Begin("Scene States",&mVisible);
        ImGui::Columns(2);
        ImGui::Text("Scene");
        ImGui::NextColumn();
        ImGui::Text("State");
        ImGui::NextColumn();

        auto pDef = mState->project->getProjectDefinition();
        auto pRunt = mState->project->getProjectRuntime();
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

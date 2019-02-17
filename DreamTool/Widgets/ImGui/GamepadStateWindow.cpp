#include "GamepadStateWindow.h"
#include "../../DTState.h"
#include "../../../DreamCore/Project/Project.h"
#include "../../../DreamCore/Project/ProjectRuntime.h"
#include "../../../DreamCore/Components/Input/InputComponent.h"


namespace DreamTool
{
    GamepadStateWindow::GamepadStateWindow
    (DTState* state, bool visible)
        : ImGuiWidget (state,visible)
    {
#ifdef DREAM_LOG
        setLogClassName("GamepadStateWindow");
#endif
    }

    GamepadStateWindow::~GamepadStateWindow
    ()
    {

    }

    void
    GamepadStateWindow::draw
    ()
    {
        if (mState->project)
        {

            ImGui::Begin("Gamepad State",&mVisible);
            auto pRuntime = mState->project->getRuntime();
            if (pRuntime)
            {
                auto inputComp = pRuntime->getInputComponent();
                if (inputComp)
                {
                    JoystickState& js = inputComp->getJoystickState();

                    ImGui::Text("Gamepad: %s",js.getName().c_str());
                    ImGui::Text("Axis");
                    {
                        ImGui::Columns(2);
                        for (int i=0; i<js.getAxisCount(); i++)
                        {
                            stringstream label;
                            label << "Asix " << i;
                            float val = js.getAxisDataPointer()[i];
                            ImGui::SliderFloat(label.str().c_str(),&val,-1.0f,1.0f);
                            ImGui::NextColumn();
                        }
                        ImGui::Columns(1);
                    }

                    ImGui::Text("Buttons");
                    {
                        ImGui::Columns(4);
                        for (int i = 0; i < js.getButtonCount(); i++ )
                        {
                            stringstream label;
                            label << "Button " << i;
                            bool val = js.getButtonDataPointer()[i];;
                            ImGui::Checkbox(label.str().c_str(),&val);
                            ImGui::NextColumn();
                        }
                        ImGui::Columns(1);
                    }

                }
            }
            ImGui::End();
        }
    }
}

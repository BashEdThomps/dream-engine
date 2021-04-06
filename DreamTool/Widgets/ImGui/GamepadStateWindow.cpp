#include "GamepadStateWindow.h"
#include "DreamToolContext.h"
#include <DreamCore.h>



namespace octronic::dream::tool
{
  GamepadStateWindow::GamepadStateWindow
  (DreamToolContext& state, bool visible)
    : ImGuiWidget (state,visible)
  {
  }

  GamepadStateWindow::~GamepadStateWindow
  ()
  {

  }

  void
  GamepadStateWindow::draw
  ()
  {
    auto& pCtxOpt = getContext().getProjectContext();
    if (pCtxOpt)
    {
      auto& pCtx = pCtxOpt.value();

      ImGui::Begin("Gamepad State",&mVisible);
      auto& pRuntOpt = pCtx.getProjectRuntime();

      if (pRuntOpt)
      {
        auto& pRunt = pRuntOpt.value();
        auto& inputComp = pRunt.getInputComponent();
        auto& js = inputComp.getJoystickState();

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
      ImGui::End();
    }
  }
}

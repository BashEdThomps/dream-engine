#include "TransformPropertiesPanel.h"

#include "PropertiesWindow.h"
#include "DreamToolContext.h"
#include "Widgets/GL/Cursor.h"

#include <imgui.h>
#include <glm/vec3.hpp>
#include <DreamCore.h>

using glm::vec3;
using octronic::dream::Transform;
using octronic::dream::SceneEntityDefinition;

namespace octronic::dream::tool
{

  TransformPropertiesPanel::TransformPropertiesPanel
  (ImGuiWidget& parent)
    : ImGuiPanel(parent)
  {

  }

  void TransformPropertiesPanel::draw()
  {
    auto& parent = static_cast<PropertiesWindow&>(getParent());
    auto& ctx = parent.getContext();

    // Tx, Rotation, Scale =================================================

    if (mTransform)
    {
      Transform& tx = mTransform.value().get();

      float yaw   = glm::degrees(tx.getYaw());
      float pitch = glm::degrees(tx.getPitch());
      float roll  = glm::degrees(tx.getRoll());

      vec3 translation = tx.getTranslation();
      vec3 scale = tx.getScale();

      bool tx_changed = false;
      tx_changed |= ImGui::DragFloat3("Translation", glm::value_ptr(translation),1.f);
      tx_changed |= ImGui::DragFloat3("Scale", glm::value_ptr(scale),1.f);
      tx_changed |= ImGui::DragFloat("X (Pitch)", &pitch, 1.f);
      tx_changed |= ImGui::DragFloat("Y (Yaw)",   &yaw,   1.f);
      tx_changed |= ImGui::DragFloat("Z (Roll)",  &roll,  1.f);

      if (tx_changed)
      {
        tx.setTranslation(translation);
        tx.setScale(scale);
        tx.setPitch(glm::radians(pitch));
        tx.setYaw(glm::radians(yaw));
        tx.setRoll(glm::radians(roll));
      }

      if (ImGui::Button("Move to Cursor"))
      {
        auto& cursor = ctx.getCursor();
        vec3 cursorPos = cursor.getTransform().getTranslation();
        tx.setTranslation(cursorPos);
      }

      ImGui::SameLine();

      if(ImGui::Button("Set as Initial"))
      {
      }

      ImGui::SameLine();

      if(ImGui::Button("Restore to Initial"))
      {
      }
    }
    else
    {
      ImGui::Text("No Transform Object Found");
    }
  }

  void
  TransformPropertiesPanel::setTransform
  (Transform& t)
  {
    mTransform = t;
  }
}

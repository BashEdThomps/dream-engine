// Maintain include order for GL Defined

#include "PropertiesWindow.h"
#include "glm/gtc/type_ptr.hpp"
#include <nfd.h>
#include "DreamToolContext.h"
#include <DreamCore.h>

using glm::degrees;
using glm::radians;
using std::exception;
using octronic::dream::FontRuntime;

namespace octronic::dream::tool
{
  PropertiesWindow::PropertiesWindow
  (DreamToolContext& proj)
    : ImGuiWidget (proj),
      mScenePropertiesPanel(*this),
      mProjectPropertiesPanel(*this),
      mSceneEntityPropertiesPanel(*this),
      mTemplateEntityPropertiesPanel(*this),
      mAssetPropertiesPanel(*this),
      mImageSize(256,256)
  {
  }

  void
  PropertiesWindow::draw
  ()
  {
    ImGui::Begin("Properties",&mVisible);

    if (!mHistory.empty())
    {
      if(ImGui::Button("<- Back")) popPropertyTarget();

      auto type = getCurrentPropertyTarget().getType();

      switch (type)
      {
        case PropertyType_None:
          break;
        case PropertyType_Project:
          mProjectPropertiesPanel.draw();
          break;
        case PropertyType_Scene:
          mScenePropertiesPanel.draw();
          break;
        case PropertyType_SceneEntity:
          mSceneEntityPropertiesPanel.draw();
          break;
        case PropertyType_TemplateEntity:
          mTemplateEntityPropertiesPanel.draw();
          break;
        case PropertyType_Asset:
          mAssetPropertiesPanel.draw();
          break;
      }
    }
    ImGui::End();
  }

  void
  PropertiesWindow::pushPropertyTarget
  (PropertyTarget target)
  {

    mHistory.push_back(target);
    LOG_INFO("PropertiesWindow: Pushed target {}",mHistory.size());
  }

  void
  PropertiesWindow::removeFromHistory
  (Definition& def)
  {
    remove_if(mHistory.begin(), mHistory.end(),
              [&](const PropertyTarget& p){ return p.getDefinition().value().get() == def; });
  }

  void
  PropertiesWindow::popPropertyTarget
  ()
  {
    mHistory.pop_back();
  }

  void
  PropertiesWindow::clearPropertyTargets
  ()
  {
    mHistory.clear();
  }

  PropertyTarget
  PropertiesWindow::getCurrentPropertyTarget
  ()
  const
  {
    if (mHistory.empty()) throw std::exception();
    return mHistory.back();
  }

  ImVec2
  PropertiesWindow::getImageSize
  ()
  const
  {
    return mImageSize;
  }
}

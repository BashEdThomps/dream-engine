// Maintain include order for GL Defined

#include "PropertiesWindow.h"
#include "glm/gtc/type_ptr.hpp"
#include <nfd.h>
#include "DreamToolContext.h"
#include <DreamCore.h>

using glm::degrees;
using glm::radians;
using std::exception;
using std::make_unique;
using octronic::dream::FontRuntime;

namespace octronic::dream::tool
{
  PropertiesWindow::PropertiesWindow
  (DreamToolContext& proj)
    : ImGuiWidget (proj,true),
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
      if(ImGui::Button("<- Back"))
      {
        popPropertyTarget();
      }
      else
      {
        auto propOpt = getCurrentPropertyTarget();

        if (propOpt)
        {
          auto& prop = propOpt.value().get();
          auto type = prop.getType();

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
      }
    }
    ImGui::End();
  }

  void
  PropertiesWindow::pushPropertyTarget
  (PropertyType type, optional<reference_wrapper<Definition>> def)
  {
    auto itr = find_if(mHistory.begin(), mHistory.end(),
                       [&](unique_ptr<PropertyTarget>& next){
        if (next->getDefinition()) {
        return next->getDefinition().value().get().getUuid() == def.value().get().getUuid();
  } return false; } );

    if (itr == mHistory.end())
    {
      mHistory.push_back(make_unique<PropertyTarget>(type,def));
    }
    else
    {
      std::swap(*itr, mHistory.back());
    }
    LOG_INFO("PropertiesWindow: Pushed target {}",mHistory.size());
  }

  void
  PropertiesWindow::removeFromHistory
  (Definition& def)
  {
    if (mHistory.empty()) return;

    auto itr = find_if(mHistory.begin(), mHistory.end(),
                       [&](unique_ptr<PropertyTarget>& p) {
        if (p->getDefinition()) {
        auto& nextDef = p->getDefinition().value().get();
        return nextDef.getUuid() == def.getUuid();
  } return false; });

    if (itr != mHistory.end()) mHistory.erase(itr);
  }

  void
  PropertiesWindow::removeFromHistoryWithChildren
  (SceneEntityDefinition& def)
  {
    if (mHistory.empty()) return;

    auto withChildren = def.getAllDescendants();
    withChildren.push_back(def);

    for (auto nextDefWrap : withChildren)
    {
      auto itr = find_if(mHistory.begin(), mHistory.end(), [&](unique_ptr<PropertyTarget>& p) {
          if (p->getDefinition()) {
          auto& nextInHistory = p->getDefinition().value().get();
          return nextInHistory.getUuid() == nextDefWrap.get().getUuid();
    } return false; });

      if (itr != mHistory.end())
      {
        mHistory.erase(itr);
      }
    }
  }

  void
  PropertiesWindow::popPropertyTarget
  ()
  {
    if (!mHistory.empty()) mHistory.pop_back();
  }

  void
  PropertiesWindow::clearPropertyTargets
  ()
  {
    mHistory.clear();
  }

  optional<reference_wrapper<PropertyTarget>>
  PropertiesWindow::getCurrentPropertyTarget
  ()
  {
    if (mHistory.empty()) return std::nullopt;
    return *mHistory.back();
  }

  ImVec2
  PropertiesWindow::getImageSize
  ()
  const
  {
    return mImageSize;
  }
}

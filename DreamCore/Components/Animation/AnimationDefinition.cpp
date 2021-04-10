#include "AnimationDefinition.h"
#include "AnimationKeyframe.h"
#include "Common/Constants.h"
#include "Common/Logger.h"

using std::lock_guard;


namespace octronic::dream
{
  // public
  AnimationDefinition::AnimationDefinition
  (ProjectDefinition& pd,
   const json& js)
    : AssetDefinition(pd,js)
  {
    if (mJson.find(Constants::ASSET_ATTR_KEYFRAMES) == mJson.end())
    {
      mJson[Constants::ASSET_ATTR_KEYFRAMES] = json::array();
    }
  }

  vector<AnimationKeyframe> // public
  AnimationDefinition::getKeyframes
  ()
  {
    vector<AnimationKeyframe> keyframes;
    for (auto js : mJson[Constants::ASSET_ATTR_KEYFRAMES])
    {
      AnimationKeyframe newKeyframe(js[Constants::KEYFRAME_ID]);
      newKeyframe.fromJson(js);
      keyframes.push_back(newKeyframe);
    }
    return keyframes;
  }

  void // public
  AnimationDefinition::addKeyframe
  (const AnimationKeyframe& kf)
  {
    mJson[Constants::ASSET_ATTR_KEYFRAMES].push_back(kf.toJson());
  }

  void // public
  AnimationDefinition::updateKeyframe
  (const AnimationKeyframe& kf)
  {
    auto itr = mJson[Constants::ASSET_ATTR_KEYFRAMES].begin();
    auto end = mJson[Constants::ASSET_ATTR_KEYFRAMES].end();
    for (;itr != end; itr++)
    {
      if ((*itr)[Constants::KEYFRAME_ID] == kf.getID())
      {
        // Time
        (*itr)[Constants::KEYFRAME_TIME] = kf.getTime();
        (*itr)[Constants::KEYFRAME_EASING_TYPE] = kf.getEasingType();
        // Translation
        (*itr)[Constants::KEYFRAME_VALUE] = kf.getValue();
        return;
      }
    }
  }

  void // public
  AnimationDefinition::removeKeyframe
  (const AnimationKeyframe& kf)
  {
    auto itr = mJson[Constants::ASSET_ATTR_KEYFRAMES].begin();
    auto end = mJson[Constants::ASSET_ATTR_KEYFRAMES].end();
    for (;itr != end; itr++)
    {
      if ((*itr)[Constants::KEYFRAME_ID] == kf.getID())
      {
        mJson[Constants::ASSET_ATTR_KEYFRAMES].erase(itr);
        return;
      }
    }
  }

  bool // public
  AnimationDefinition::getRelative
  ()
  const
  {
    if (!mJson[Constants::ANIMATION_RELATIVE].is_boolean())
    {
      return false;
    }
    return mJson[Constants::ANIMATION_RELATIVE];
  }

  void // public
  AnimationDefinition::setRelative
  (bool relative)
  {
    mJson[Constants::ANIMATION_RELATIVE] = relative;
  }

  int // public
  AnimationDefinition::nextKeyframeID
  ()
  {
    int maxID = 0;
    auto itr = mJson[Constants::ASSET_ATTR_KEYFRAMES].begin();
    auto end = mJson[Constants::ASSET_ATTR_KEYFRAMES].end();
    for (;itr != end; itr++)
    {
      int nextID = (*itr)[Constants::KEYFRAME_ID];
      if (nextID > maxID)
      {
        maxID = nextID;
      }
    }
    return maxID+1;
  }
}

/*
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#include "AnimationDefinition.h"
#include "AnimationKeyframe.h"
#include "Common/Constants.h"
#include "Common/Logger.h"

namespace octronic::dream
{
    AnimationDefinition::AnimationDefinition
    (ProjectDefinition* pd, const json& js)
        : AssetDefinition(pd,js)
    {
        if (mJson[Constants::ASSET_ATTR_KEYFRAMES].is_null())
        {
            mJson[Constants::ASSET_ATTR_KEYFRAMES] = json::array();
        }
    }

    AnimationDefinition::~AnimationDefinition
    ()
    {
        LOG_TRACE("AnimationDefinition: Destructing");
    }

    vector<AnimationKeyframe>
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

    void
    AnimationDefinition::addKeyframe
    (const AnimationKeyframe& kf)
    {
        mJson[Constants::ASSET_ATTR_KEYFRAMES].push_back(kf.toJson());
    }

    void
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
               (*itr)[Constants::KEYFRAME_TRANSLATION][Constants::X] = kf.getTranslation().x;
               (*itr)[Constants::KEYFRAME_TRANSLATION][Constants::Y] = kf.getTranslation().y;
               (*itr)[Constants::KEYFRAME_TRANSLATION][Constants::Z] = kf.getTranslation().z;
               // Rotation
               (*itr)[Constants::KEYFRAME_ROTATION][Constants::X] = kf.getRotation().x;
               (*itr)[Constants::KEYFRAME_ROTATION][Constants::Y] = kf.getRotation().y;
               (*itr)[Constants::KEYFRAME_ROTATION][Constants::Z] = kf.getRotation().z;
               // Translation
               (*itr)[Constants::KEYFRAME_SCALE][Constants::X] = kf.getScale().x;
               (*itr)[Constants::KEYFRAME_SCALE][Constants::Y] = kf.getScale().y;
               (*itr)[Constants::KEYFRAME_SCALE][Constants::Z] = kf.getScale().z;
               return;
           }
        }
    }

    void
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

    bool
    AnimationDefinition::getRelative
    ()
    {
        if (!mJson[Constants::ANIMATION_RELATIVE].is_boolean())
        {
            mJson[Constants::ANIMATION_RELATIVE] = false;
        }
        return mJson[Constants::ANIMATION_RELATIVE];
    }

    void
    AnimationDefinition::setRelative
    (bool relative)
    {
       mJson[Constants::ANIMATION_RELATIVE] = relative;
    }

    int
    AnimationDefinition::nextKeyframeID
    ()
    {
        int maxId = 0;
        auto itr = mJson[Constants::ASSET_ATTR_KEYFRAMES].begin();
        auto end = mJson[Constants::ASSET_ATTR_KEYFRAMES].end();
        for (;itr != end; itr++)
        {
            int nextId = (*itr)[Constants::KEYFRAME_ID];
            if (nextId > maxId)
            {
               maxId = nextId;
            }
        }
        return maxId+1;
    }
}

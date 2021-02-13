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

using std::lock_guard;


namespace octronic::dream
{
    // public
    AnimationDefinition::AnimationDefinition
    (ProjectDefinition* pd, const json& js)
        : AssetDefinition("AnimationDefinition",pd,js)
    {
        if (mJson.find(Constants::ASSET_ATTR_KEYFRAMES) == mJson.end())
        {
            mJson[Constants::ASSET_ATTR_KEYFRAMES] = json::array();
        }
    }

    // public
    AnimationDefinition::~AnimationDefinition()
    {
        LOG_TRACE("AnimationDefinition: Destructing");
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
                (*itr)[Constants::KEYFRAME_TRANSLATION] = kf.getTranslation().toJson();
                // Rotation
                (*itr)[Constants::KEYFRAME_ROTATION] = kf.getRotation().toJson();
                // Translation
                (*itr)[Constants::KEYFRAME_SCALE] = kf.getScale().toJson();
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
    {
        if (!mJson[Constants::ANIMATION_RELATIVE].is_boolean())
        {
            mJson[Constants::ANIMATION_RELATIVE] = false;
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

/*
 * AnimationDefinition.cpp
 *
 * Created: 11 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
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

namespace Dream
{
    AnimationDefinition::AnimationDefinition
    (ProjectDefinition* pd, json js)
        : IAssetDefinition(pd,js)
    {
        if (js[Constants::ASSET_ATTR_KEYFRAMES].is_null())
        {
            js[Constants::ASSET_ATTR_KEYFRAMES] = json::array();
        }
    }

    AnimationDefinition::~AnimationDefinition()
    {

    }

    vector<AnimationKeyframe>
    AnimationDefinition::getKeyframes
    ()
    {
        vector<AnimationKeyframe> keyframes;
        for (auto js : mJson[Constants::ASSET_ATTR_KEYFRAMES])
        {
            AnimationKeyframe newKeyframe;
            newKeyframe.fromJson(js);
            keyframes.push_back(newKeyframe);
        }
        return keyframes;
    }

    void AnimationDefinition::addKeyframe(AnimationKeyframe kf)
    {
        removeKeyframe(kf);
        mJson[Constants::ASSET_ATTR_KEYFRAMES].push_back(kf.toJson());
    }

    void AnimationDefinition::removeKeyframe(AnimationKeyframe kf)
    {
        auto itr = mJson[Constants::ASSET_ATTR_KEYFRAMES].begin();
        auto end = mJson[Constants::ASSET_ATTR_KEYFRAMES].end();
        for (;itr != end; itr++)
        {
           if ((*itr)[Constants::KEYFRAME_TIME] == kf.getTime())
           {
               break;
           }
        }

        if (itr != end)
        {
            mJson[Constants::ASSET_ATTR_KEYFRAMES].erase(itr);
        }
    }
}

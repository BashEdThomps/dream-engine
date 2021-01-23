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

#pragma once

#include "Components/AssetDefinition.h"

namespace octronic::dream
{
    class AnimationKeyframe;
    class AnimationDefinition : public AssetDefinition
    {
    public:
        AnimationDefinition(ProjectDefinition*, const json&);
        ~AnimationDefinition() override;

        vector<AnimationKeyframe> getKeyframes();

        void addKeyframe(const AnimationKeyframe& kf);
        void updateKeyframe(const AnimationKeyframe& kf);
        void removeKeyframe(const AnimationKeyframe &kf);

        bool getRelative();
        void setRelative(bool relative);

        int nextKeyframeID();

        int getEasingType();
        void setEasingType(int easingType);
    };
}

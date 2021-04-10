#pragma once

#include "Components/AssetDefinition.h"

namespace octronic::dream
{
    class AnimationKeyframe;
    class AnimationDefinition : public AssetDefinition
    {
    public:
        AnimationDefinition(ProjectDefinition&, const json&) ;

        AnimationDefinition(AnimationDefinition&&) = default;
        AnimationDefinition& operator=(AnimationDefinition&&) = default;


        vector<AnimationKeyframe> getKeyframes();

        void addKeyframe(const AnimationKeyframe& kf);
        void updateKeyframe(const AnimationKeyframe& kf);
        void removeKeyframe(const AnimationKeyframe &kf);

        bool getRelative() const;
        void setRelative(bool relative);

        int nextKeyframeID();
    };
}

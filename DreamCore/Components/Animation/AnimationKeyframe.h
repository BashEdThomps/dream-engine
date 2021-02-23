#pragma once

#include "AnimationEasing.h"

#include "Math/Transform.h"

#include <json.hpp>

using nlohmann::json;

namespace octronic::dream
{
    class AnimationKeyframe
    {
    public:
        AnimationKeyframe(int id);
        ~AnimationKeyframe();

        long getTime() const;
        void setTime(long time);

        Transform getTransform() const;
        void setTransform(const Transform& transform);

        void fromJson(const json& js);
        json toJson() const ;

        int getID() const;
        void setID(int iD);

        bool operator>  (const AnimationKeyframe &other) const;
        bool operator<= (const AnimationKeyframe &other) const;
        bool operator<  (const AnimationKeyframe &other) const;
        bool operator>= (const AnimationKeyframe &other) const;
        bool operator== (const AnimationKeyframe &other) const;

        AnimationEasing::EasingType getEasingType() const;
        void setEasingType(AnimationEasing::EasingType type);

    private:
        int mID;
        long mTime;
        Transform mTransform;
        AnimationEasing::EasingType mEasingType;
    };
}

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

        float getValue() const;
        void setValue(float transform);

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
        float mValue;
        AnimationEasing::EasingType mEasingType;
    };
}

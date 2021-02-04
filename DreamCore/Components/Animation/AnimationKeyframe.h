#pragma once

#include "Common/Vector.h"
#include "AnimationEasing.h"

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

        Vector3 getTranslation() const;
        void setTranslation(const Vector3& translation);

        Vector3 getRotation() const;
        void setRotation(const Vector3& rotation);

        Vector3 getScale() const;
        void setScale(const Vector3& scale);

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
        Vector3 mTranslation;
        Vector3 mRotation;
        Vector3 mScale;
        AnimationEasing::EasingType mEasingType;
    };
}

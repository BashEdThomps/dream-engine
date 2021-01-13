#pragma once

#include "AnimationEasing.h"

#include <json.hpp>
#include <glm/vec3.hpp>

using nlohmann::json;
using glm::vec3;

namespace Dream
{
    class AnimationKeyframe
    {
    public:
        AnimationKeyframe(int id);
        ~AnimationKeyframe();

        long getTime() const;
        void setTime(long time);

        vec3 getTranslation() const;
        void setTranslation(const vec3& translation);

        vec3 getRotation() const;
        void setRotation(const vec3& rotation);

        vec3 getScale() const;
        void setScale(const vec3& scale);

        void fromJson(const json& js);
        json toJson() const ;

        int getID() const;
        void setID(int iD);

        bool operator>  (const AnimationKeyframe &other) const;
        bool operator<= (const AnimationKeyframe &other) const;
        bool operator<  (const AnimationKeyframe &other) const;
        bool operator>= (const AnimationKeyframe &other) const;
        bool operator== (const AnimationKeyframe &other) const;

        AnimationEasing::Type getEasingType() const;
        void setEasingType(AnimationEasing::Type type);

    private:
        int mID;
        long mTime;
        vec3 mTranslation;
        vec3 mRotation;
        vec3 mScale;
        AnimationEasing::Type mEasingType;
    };
}

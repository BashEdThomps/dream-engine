#pragma once

#include <glm/vec3.hpp>
#include "../../Common/DreamObject.h"
#include "../../deps/json/json.hpp"
#include "AnimationEasing.h"

using namespace nlohmann;
using namespace glm;

namespace Dream
{
    class AnimationKeyframe : public DreamObject
    {
    public:
        AnimationKeyframe(int id);
        ~AnimationKeyframe() override;

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

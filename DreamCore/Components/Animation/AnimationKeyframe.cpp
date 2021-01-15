#include "AnimationKeyframe.h"
#include "AnimationRuntime.h"
#include "Common/Constants.h"
#include "Common/Logger.h"

namespace Dream
{
    AnimationKeyframe::AnimationKeyframe
    (int id)
        : mID(id),
          mTime(0),
          mTranslation(vec3(0.0f)),
          mRotation(vec3(0.0f)),
          mScale(vec3(1.0f)),
          mEasingType(AnimationEasing::Type::EasingBounceInOut)
    {

    }

    AnimationKeyframe::~AnimationKeyframe
    ()
    {
        LOG_TRACE("AnimationKeyFrame: Destructing");
    }

    long
    AnimationKeyframe::getTime
    ()
    const
    {
        return mTime;
    }

    void
    AnimationKeyframe::setTime
    (long time)
    {
        mTime = time;
    }

    void
    AnimationKeyframe::fromJson
    (const json &js)
    {
        if (!js[Constants::KEYFRAME_ID].is_null())
        {
            mID = js[Constants::KEYFRAME_ID];
        }

        if (!js[Constants::KEYFRAME_TIME].is_null())
        {
            mTime = js[Constants::KEYFRAME_TIME];
        }

        if (!js[Constants::KEYFRAME_EASING_TYPE].is_null())
        {
            mEasingType = static_cast<AnimationEasing::Type>(js[Constants::KEYFRAME_EASING_TYPE]);
        }

        if (!js[Constants::KEYFRAME_TRANSLATION].is_null())
        {
            mTranslation.x = js[Constants::KEYFRAME_TRANSLATION][Constants::X];
            mTranslation.y = js[Constants::KEYFRAME_TRANSLATION][Constants::Y];
            mTranslation.z = js[Constants::KEYFRAME_TRANSLATION][Constants::Z];
        }

        if (!js[Constants::KEYFRAME_ROTATION].is_null())
        {
            mRotation.x = js[Constants::KEYFRAME_ROTATION][Constants::X];
            mRotation.y = js[Constants::KEYFRAME_ROTATION][Constants::Y];
            mRotation.z = js[Constants::KEYFRAME_ROTATION][Constants::Z];
        }

        if (!js[Constants::KEYFRAME_SCALE].is_null())
        {
            mScale.x = js[Constants::KEYFRAME_SCALE][Constants::X];
            mScale.y = js[Constants::KEYFRAME_SCALE][Constants::Y];
            mScale.z = js[Constants::KEYFRAME_SCALE][Constants::Z];
        }
    }

    json
    AnimationKeyframe::toJson
    ()
    const
    {
        json js = json::object();
        js[Constants::KEYFRAME_ID] = mID;
        js[Constants::KEYFRAME_TIME] = mTime;
        js[Constants::KEYFRAME_EASING_TYPE] = static_cast<unsigned int>(mEasingType);

        js[Constants::KEYFRAME_TRANSLATION] = json::object();
        js[Constants::KEYFRAME_TRANSLATION][Constants::X] = mTranslation.x;
        js[Constants::KEYFRAME_TRANSLATION][Constants::Y] = mTranslation.y;
        js[Constants::KEYFRAME_TRANSLATION][Constants::Z] = mTranslation.z;

        js[Constants::KEYFRAME_ROTATION] = json::object();
        js[Constants::KEYFRAME_ROTATION][Constants::X] = mRotation.x;
        js[Constants::KEYFRAME_ROTATION][Constants::Y] = mRotation.y;
        js[Constants::KEYFRAME_ROTATION][Constants::Z] = mRotation.z;

        js[Constants::KEYFRAME_SCALE] = json::object();
        js[Constants::KEYFRAME_SCALE][Constants::X] = mScale.x;
        js[Constants::KEYFRAME_SCALE][Constants::Y] = mScale.y;
        js[Constants::KEYFRAME_SCALE][Constants::Z] = mScale.z;

        return js;
    }

    bool
    AnimationKeyframe::operator>
    (const AnimationKeyframe& other)
    const
    {
        return mTime > other.mTime;
    }

    bool
    AnimationKeyframe::operator<=
    (const AnimationKeyframe& other)
    const
    {
        return mTime <= other.mTime;
    }

    bool
    AnimationKeyframe::operator<
    (const AnimationKeyframe& other)
    const
    {
        return mTime < other.mTime;
    }

    bool
    AnimationKeyframe::operator>=
    (const AnimationKeyframe& other)
    const
    {
        return mTime >= other.mTime;
    }

    bool
    AnimationKeyframe::operator==
    (const AnimationKeyframe& other)
    const
    {
       return mID == other.mID;
    }

    int
    AnimationKeyframe::getID
    ()
    const
    {
        return mID;
    }

    void
    AnimationKeyframe::setID
    (int iD)
    {
        mID = iD;
    }

    vec3
    AnimationKeyframe::getTranslation
    ()
    const
    {
        return mTranslation;
    }

    void
    AnimationKeyframe::setTranslation
    (const vec3& translation)
    {
        mTranslation = translation;
    }

    vec3
    AnimationKeyframe::getRotation
    ()
    const
    {
        return mRotation;
    }

    void
    AnimationKeyframe::setRotation
    (const vec3& rotation)
    {
        mRotation = rotation;
    }

    vec3
    AnimationKeyframe::getScale
    ()
    const
    {
        return mScale;
    }

    void
    AnimationKeyframe::setScale
    (const vec3& scale)
    {
        mScale = scale;
    }

    AnimationEasing::Type
    AnimationKeyframe::getEasingType
    ()
    const
    {
        return mEasingType;
    }

    void
    AnimationKeyframe::setEasingType
    (AnimationEasing::Type type)
    {
        mEasingType = type;
    }
}

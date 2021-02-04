#include "AnimationKeyframe.h"
#include "AnimationRuntime.h"
#include "Common/Constants.h"
#include "Common/Logger.h"

namespace octronic::dream
{
    AnimationKeyframe::AnimationKeyframe
    (int id)
        : mID(id),
          mTime(0),
          mTranslation(Vector3(0.0f)),
          mRotation(Vector3(0.0f)),
          mScale(Vector3(1.0f)),
          mEasingType(AnimationEasing::EasingType::EasingBounceInOut)
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
        if (js.find(Constants::KEYFRAME_ID) != js.end())
        {
            mID = js[Constants::KEYFRAME_ID];
        }

        if (js.find(Constants::KEYFRAME_TIME) != js.end())
        {
            mTime = js[Constants::KEYFRAME_TIME];
        }

        if (js.find(Constants::KEYFRAME_EASING_TYPE) != js.end())
        {
            mEasingType = static_cast<AnimationEasing::EasingType>(js[Constants::KEYFRAME_EASING_TYPE]);
        }

        if (js.find(Constants::KEYFRAME_TRANSLATION) != js.end())
        {
            mTranslation = Vector3(js[Constants::KEYFRAME_TRANSLATION]);
        }

        if (js.find(Constants::KEYFRAME_ROTATION) != js.end())
        {
            mRotation = Vector3(js[Constants::KEYFRAME_ROTATION]);
        }

        if (js.find(Constants::KEYFRAME_SCALE) != js.end())
        {
            mScale = Vector3(js[Constants::KEYFRAME_SCALE]);
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
        js[Constants::KEYFRAME_TRANSLATION] = mTranslation.toJson();
        js[Constants::KEYFRAME_ROTATION] = mRotation.toJson();
        js[Constants::KEYFRAME_SCALE] = mScale.toJson();

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

    Vector3
    AnimationKeyframe::getTranslation
    ()
    const
    {
        return mTranslation;
    }

    void
    AnimationKeyframe::setTranslation
    (const Vector3& translation)
    {
        mTranslation = translation;
    }

    Vector3
    AnimationKeyframe::getRotation
    ()
    const
    {
        return mRotation;
    }

    void
    AnimationKeyframe::setRotation
    (const Vector3& rotation)
    {
        mRotation = rotation;
    }

    Vector3
    AnimationKeyframe::getScale
    ()
    const
    {
        return mScale;
    }

    void
    AnimationKeyframe::setScale
    (const Vector3& scale)
    {
        mScale = scale;
    }

    AnimationEasing::EasingType
    AnimationKeyframe::getEasingType
    ()
    const
    {
        return mEasingType;
    }

    void
    AnimationKeyframe::setEasingType
    (AnimationEasing::EasingType type)
    {
        mEasingType = type;
    }
}

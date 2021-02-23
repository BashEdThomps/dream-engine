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

        if (js.find(Constants::KEYFRAME_TRANSFORM) != js.end())
        {
            mTransform = Transform(js[Constants::KEYFRAME_TRANSFORM]);
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
        js[Constants::KEYFRAME_TRANSFORM] = mTransform.toJson();
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

    Transform
    AnimationKeyframe::getTransform
    ()
    const
    {
        return mTransform;
    }

    void
    AnimationKeyframe::setTransform
    (const Transform& transform)
    {
        mTransform = transform;
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

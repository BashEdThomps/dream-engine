#include "AnimationKeyframe.h"
#include "AnimationInstance.h"

namespace Dream
{
    AnimationKeyframe::AnimationKeyframe
    ()
        : DreamObject("AnimationKeyframe")
    {

    }

    AnimationKeyframe::~AnimationKeyframe
    ()
    {

    }

    void
    AnimationKeyframe::setValue
    (KeyframeTarget target, vec3 value)
    {
        mValues.insert(std::pair<KeyframeTarget,vec3>(target,value));
    }

    vec3
    AnimationKeyframe::getValue
    (KeyframeTarget target)
    {
        if (mValues.find(target) != mValues.end())
        {
            return mValues[target];
        }
        return vec3(0.0f);
    }

    long
    AnimationKeyframe::getTime()
    const
    {
        return mTime;
    }

    void
    AnimationKeyframe::setTime(long time)
    {
        mTime = time;
    }

    void
    AnimationKeyframe::fromJson
    (json js)
    {
        if (!js[Constants::KEYFRAME_TIME].is_null())
        {
            mTime = js[Constants::KEYFRAME_TIME];
        }

        if (!js[Constants::KEYFRAME_VALUES].is_null())
        {
            for (json valObj : js[Constants::KEYFRAME_VALUES])
            {
                KeyframeTarget target = js[Constants::KEYFRAME_TARGET];
                vec3 value
                {
                    js[Constants::KEYFRAME_VALUE][Constants::X],
                    js[Constants::KEYFRAME_VALUE][Constants::Y],
                    js[Constants::KEYFRAME_VALUE][Constants::Z],
                };
                mValues.insert(pair<KeyframeTarget,vec3>(target,value));
            }
        }
    }

    json
    AnimationKeyframe::toJson
    ()
    {
        json js = json::object();
        js[Constants::KEYFRAME_TIME] = mTime;
        js[Constants::KEYFRAME_VALUES] = json::array();

        for (auto kfVal : mValues)
        {
            vec3 val = kfVal.second;
            js[Constants::KEYFRAME_TARGET] = static_cast<int>(kfVal.first);
            js[Constants::KEYFRAME_VALUE][Constants::X] = val.x;
            js[Constants::KEYFRAME_VALUE][Constants::Y] = val.y;
            js[Constants::KEYFRAME_VALUE][Constants::Z] = val.z;
        }

        return js;
    }

    bool
    AnimationKeyframe::operator>
    (const AnimationKeyframe& other)
    {
        return mTime > other.mTime;
    }

    bool
    AnimationKeyframe::operator<=
    (const AnimationKeyframe& other)
    {
        return mTime <= other.mTime;
    }

    bool
    AnimationKeyframe::operator<
    (const AnimationKeyframe& other)
    {

        return mTime < other.mTime;
    }

    bool
    AnimationKeyframe::operator>=
    (const AnimationKeyframe& other)
    {
        return mTime >= other.mTime;
    }

    bool
    AnimationKeyframe::operator==
    (const AnimationKeyframe& other)
    {
       return mTime == other.mTime;
    }
}

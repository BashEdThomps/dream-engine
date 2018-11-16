#pragma once

#include "../../Common/DreamObject.h"
#include <glm/vec3.hpp>
#include "../../deps/json/json.hpp"

using namespace nlohmann;
using namespace glm;

namespace Dream
{
    class AnimationInstance;

    enum KeyframeTarget
    {
        TransformTranslation = 0,
        TransformRotation,
        TransformScale,
    };

    class AnimationKeyframe : public DreamObject
    {
    public:
        AnimationKeyframe(AnimationInstance* parent);
        ~AnimationKeyframe() override;

        void setValue(KeyframeTarget,vec3);
        vec3 getValue(KeyframeTarget);

        long getTime() const;
        void setTime(long time);

        AnimationInstance* getParent() const;

        void fromJson(json js);
        json toJson();

        bool operator>  (const AnimationKeyframe &other);
        bool operator<= (const AnimationKeyframe &other);
        bool operator<  (const AnimationKeyframe &other);
        bool operator>= (const AnimationKeyframe &other);

    private:
        AnimationInstance* mParent;
        map<KeyframeTarget,vec3> mValues;
        long mTime;
    };

}

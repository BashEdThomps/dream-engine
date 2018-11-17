#pragma once

#include "../../Common/DreamObject.h"
#include <glm/vec3.hpp>
#include "../../deps/json/json.hpp"

using namespace nlohmann;
using namespace glm;

namespace Dream
{
    enum KeyframeTarget
    {
        TransformTranslation = 0,
        TransformRotation,
        TransformScale,
    };

    class AnimationKeyframe : public DreamObject
    {
    public:
        AnimationKeyframe();
        ~AnimationKeyframe() override;

        void setValue(KeyframeTarget,vec3);
        vec3 getValue(KeyframeTarget);

        long getTime() const;
        void setTime(long time);

        void fromJson(json js);
        json toJson();

        bool operator>  (const AnimationKeyframe &other);
        bool operator<= (const AnimationKeyframe &other);
        bool operator<  (const AnimationKeyframe &other);
        bool operator>= (const AnimationKeyframe &other);
        bool operator== (const AnimationKeyframe &other);

    private:
        map<KeyframeTarget,vec3> mValues;
        long mTime;
    };

}

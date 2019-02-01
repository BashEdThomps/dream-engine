#pragma once

#include "../../../deps/tweeny/tweeny.h"
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::function;

namespace Dream
{
    class AnimationEasing
    {
    public:
        enum Type
        {
            EasingLinear = 0,

            EasingQuadraticIn,
            EasingQuadraticOut,
            EasingQuadraticInOut,

            EasingCubicIn,
            EasingCubicOut,
            EasingCubicInOut,

            EasingQuarticIn,
            EasingQuarticOut,
            EasingQuarticInOut,

            EasingQuinticIn,
            EasingQuinticOut,
            EasingQuinticInOut,

            EasingSinusoidalIn,
            EasingSinusoidalOut,
            EasingSinusoidalInOut,

            EasingExponentialIn,
            EasingExponentialOut,
            EasingExponentialInOut,

            EasingCircularIn,
            EasingCircularOut,
            EasingCircularInOut,

            EasingBounceIn,
            EasingBounceOut,
            EasingBounceInOut,

            EasingElasticIn,
            EasingElasticOut,
            EasingElasticInOut,

            EasingBackIn,
            EasingBackOut,
            EasingBackInOut
        };


        static const vector<string> EasingNames;
        static AnimationEasing::Type getEasingType(string);
        static const string getEasingName(AnimationEasing::Type easing);
    };
}

#pragma once

#include <tweeny.h>

#include <string>
#include <vector>

using std::string;
using std::vector;
using std::function;

namespace octronic::dream
{
    class AnimationEasing
    {
    public:
        enum EasingType
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
        static AnimationEasing::EasingType getEasingType(const string &);
        static const string getEasingName(AnimationEasing::EasingType easing);
    };
}

#include "AnimationEasing.h"

namespace Dream
{



    const string
    AnimationEasing::getEasingName
    (AnimationEasing::Type easing)
    {
        return EasingNames.at(easing);
    }

    AnimationEasing::Type
    AnimationEasing::getEasingType
    (std::string str)
    {
       auto namePos = std::find(EasingNames.begin(), EasingNames.end(),str);
       if (namePos != EasingNames.end())
       {
          return static_cast<AnimationEasing::Type>(std::distance(EasingNames.begin(), namePos));
       }
       return  Type::EasingLinear;
    }

    const vector<string> AnimationEasing::EasingNames =
    {
        "Linear",
        "QuadraticIn",
        "QuadraticOut",
        "QuadraticInOut",
        "CubicIn",
        "CubicOut",
        "CubicInOut",
        "QuarticIn",
        "QuarticOut",
        "QuarticInOut",
        "QuinticIn",
        "QuinticOut",
        "QuinticInOut",
        "SinusoidalIn",
        "SinusoidalOut",
        "SinusoidalInOut",
        "ExponentialIn",
        "ExponentialOut",
        "ExponentialInOut",
        "CircularIn",
        "CircularOut",
        "CircularInOut",
        "BounceIn",
        "BounceOut",
        "BounceInOut",
        "ElasticIn",
        "ElasticOut",
        "ElasticInOut",
        "BackIn",
        "BackOut",
        "BackInOut"
    };
}

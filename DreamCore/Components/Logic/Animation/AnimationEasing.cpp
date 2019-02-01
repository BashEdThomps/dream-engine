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

    const vector<string>
    AnimationEasing::EasingNames =
    {
        "Linear",
        "Quadratic In",
        "Quadratic Out",
        "Quadratic In/Out",
        "Cubic In",
        "Cubic Out",
        "Cubic In/Out",
        "Quartic In",
        "Quartic Out",
        "Quartic In/Out",
        "Quintic In",
        "Quintic Out",
        "Quintic In/Out",
        "Sinusoidal In",
        "Sinusoidal Out",
        "Sinusoidal In/Out",
        "Exponential In",
        "Exponential Out",
        "Exponential In/Out",
        "Circular In",
        "Circular Out",
        "Circular In/Out",
        "Bounce In",
        "Bounce Out",
        "Bounce In/Out",
        "Elastic In",
        "Elastic Out",
        "Elastic In/Out",
        "Back In",
        "Back Out",
        "Back In/Out"
    };
}

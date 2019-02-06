#include "ModelAnimation.h"


namespace Dream
{
    ModelAnimation::ModelAnimation
    ()
        :DreamObject ("ModelAnimation")
    {
        #ifdef DREAM_LOG
        getLog()->trace("Constructing");
        #endif
    }

    ModelAnimation::~ModelAnimation
    ()
    {
        #ifdef DREAM_LOG
        getLog()->trace("Destructing");
        #endif
    }
}

#ifndef DREAMJSONCONFIG_H
#define DREAMJSONCONFIG_H

#include "../../dream-animation/JsonConfig.h"
#include "../../dream-scenegraph/JsonConfig.h"

namespace Dream {
    class JsonConfig {
    private:
        DreamAnimation::JsonConfig  *mAnimationConfig;
        DreamScenegraph::JsonConfig *mScenegraphConfig;
    };
}

#endif  // DREAMJSONCONFIG_H

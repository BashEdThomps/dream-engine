#ifndef DREAMJSONCONFIG_H
#define DREAMJSONCONFIG_H

#include "../../dream-animation/daJsonConfig.h"
#include "../../dream-scenegraph/dsgJsonConfig.h"
#include "../../dream-collision-world/dcwJsonConfig.h"

typedef struct {
    daJsonConfig  *animationConfig;
    dcwJsonConfig *collisionWorldConfig;
    dsgJsonConfig *scenegraphConfig;
} dreamJsonConfig;

#endif  // DREAMJSONCONFIG_H

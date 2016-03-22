#ifndef DREAM_H
#define DREAME_H

#include "../../dream-collision-world/src/cpp/CWrapper/dcwCollisionWorld.h"
#include "../../dream-scenegraph/src/dsgScenegraph.h"
#include "../../dream-animation/src/daDreamAnimation.h"

typedef struct {
	dsgScenegraph     *scenegraph;
	dcwCollisionWorld *collisionWorld;
	daDreamAnimation  *animations;
} dreamData;

dreamData* dreamEngineDataCreate(int,int,int);
void dreamEngineDataDestroy(dreamData*);

#endif // DREAM_H

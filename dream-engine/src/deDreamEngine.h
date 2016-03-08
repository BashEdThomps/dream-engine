#ifndef DE_DREAMENGINE_H
#define DE_DREAMENGINE_H

#include "../../dream-collision-world/src/cpp/CWrapper/dcwCollisionWorld.h"
#include "../../dream-scenegraph/src/dsgScenegraph.h"

typedef struct {
	dsgScenegraph     *scenegraph;
	dcwCollisionWorld *collisionWorld;
} deDreamEngineData;

deDreamEngineData* deDreamEngineDataCreate(void);
void deDreamEngineDataDestroy(deDreamEngineData*);

#endif // DE_DREAMENGINE_H

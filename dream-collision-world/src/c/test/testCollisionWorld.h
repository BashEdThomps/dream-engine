#ifndef TEST_COLLISION_WORLD_H
#define TEST_COLLISION_WORLD_H

#include "../../cpp/CWrapper/dcwCollisionWorld.h"

void testCollisionWorld(void);
void testCollisionWorldCreateWorld(void);
void testColWorldAddObject(void);
void testColWorldRemoveObject(void);
void testColWorldPerformDiscreteCollisionDetection(void);
void testColWorldTwoBoxCollision(void);
dcwCollisionWorld* _createCollisionWorld(void);

#endif // TEST_COLLISION_WORLD_H

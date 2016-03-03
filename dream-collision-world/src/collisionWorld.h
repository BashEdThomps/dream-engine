#ifndef COLLISION_WORLD_H
#define COLLISION_WORLD_H

#include "collisionObject.h"

#define COLLISION_OBJECTS_SZ 256

typedef struct {
	char* name;
	dsgCollisionObject_t objects[COLLISION_OBJECTS_SZ];
} dsgCollisionWorld_t;

#endif // COLLISION_WORLD_H

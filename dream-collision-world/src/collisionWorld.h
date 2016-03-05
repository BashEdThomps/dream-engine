#ifndef COLLISION_WORLD_H
#define COLLISION_WORLD_H

#include "collisionObject.h"

#define COLLISION_OBJECTS_SZ 256

typedef struct {
	char* name;
	dsgCollisionObject_t objects[COLLISION_OBJECTS_SZ];
} dsgCollisionWorld_t;

// Manage Objects
void dsgColWorldAddObject                   (dsgCollisionWorld_t*, dsgCollisionObject_t*);
int  dsgColWorldGetNextAvailableObjectIndex (dsgCollisionWorld_t*);
void dsgColWorldRemoveObject                (dsgCollisionWorld_t*, dsgCollisionObject_t*);

// Udate World
void dsgColWorldUpdate(dsgCollisionWorld_t*, long timeDeltaMs);

#endif // COLLISION_WORLD_H

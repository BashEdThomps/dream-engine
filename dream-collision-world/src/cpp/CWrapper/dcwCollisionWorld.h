#ifndef COLLISION_WORLD_H
#define COLLISION_WORLD_H

#ifdef __cplusplus
#include "../BulletCollision/BroadphaseCollision/btDispatcher.h"
#include "../BulletCollision/BroadphaseCollision/btBroadphaseInterface.h"
#include "../BulletCollision/CollisionDispatch/btCollisionConfiguration.h"
#endif

#include "dcwVector3.h"
#include "dcwCollisionObject.h"

struct dcwCollisionWorld;
typedef struct dcwCollisionWorld dcwCollisionWorld;

#ifdef __cplusplus
extern "C" {
#endif

	dcwCollisionWorld* dcwCollisionWorldCreate                 ();
	void               dcwCollisionWorldDestroy                (dcwCollisionWorld*);
	void               dcwCollisionWorldAddCollisionObject     (dcwCollisionWorld*, dcwCollisionObject*);
	void               dcwCollisionWorldRemoveCollisionObject  (dcwCollisionWorld*, dcwCollisionObject*);
	int                dcwCollisionWorldGetNumCollisionObjects (dcwCollisionWorld*);

#ifdef __cplusplus
}
#endif

#endif // COLLISION_WORLD_H

#ifndef COLLISION_WORLD_H
#define COLLISION_WORLD_H

#ifdef __cplusplus
#include "../BulletCollision/BroadphaseCollision/btDispatcher.h"
#include "../BulletCollision/BroadphaseCollision/btBroadphaseInterface.h"
#include "../BulletCollision/CollisionDispatch/btCollisionConfiguration.h"
#endif

struct dsgCollisionWorld_t;
typedef struct dsgCollisionWorld_t dsgCollisionWorld_t;

#ifdef __cplusplus
extern "C" {
#endif

	dsgCollisionWorld_t *dsgCollisionWorldCreate();
	void dsgCollisionWorldDestroy(dsgCollisionWorld_t *v);

#ifdef __cplusplus
}
#endif

#endif // COLLISION_WORLD_H

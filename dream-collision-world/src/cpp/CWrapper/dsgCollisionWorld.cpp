#ifdef __cplusplus
#include "../BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "../BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#include "../BulletCollision/BroadphaseCollision/btAxisSweep3.h"
#include "../BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "../LinearMath/btVector3.h"
#endif

#include "dsgCollisionWorld.h"

#ifdef __cplusplus
extern "C" {
#endif

	dsgCollisionWorld_t *dsgCollisionWorldCreate() {
		const btVector3 aabbMin(-100,-100,-100);
		const btVector3 aabbMax(-100,-100,-100);
		btBroadphaseInterface* broadphasePairCache = new btAxisSweep3(aabbMin,aabbMax);//BroadphaseInterface();
		btCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
		btDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
		return reinterpret_cast<dsgCollisionWorld_t*>(
			new btCollisionWorld(dispatcher, broadphasePairCache, collisionConfiguration)
		);
	}

	void dsgCollisionWorldDestroy(dsgCollisionWorld_t *v)
	{
		delete reinterpret_cast<btCollisionWorld*>(v);
		return;
	}

#ifdef __cplusplus
}
#endif

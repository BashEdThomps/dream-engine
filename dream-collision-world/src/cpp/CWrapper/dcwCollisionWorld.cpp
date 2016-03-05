#ifdef __cplusplus
#include "../BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "../BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#include "../BulletCollision/BroadphaseCollision/btAxisSweep3.h"
#include "../BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "../LinearMath/btVector3.h"
#endif

#include "dcwCollisionWorld.h"

#ifdef __cplusplus
extern "C" {
#endif
	dcwCollisionWorld* dcwCollisionWorldCreate() {
		const btVector3 aabbMin(-100,-100,-100);
		const btVector3 aabbMax(-100,-100,-100);

		btBroadphaseInterface*    broadphasePairCache    = new btAxisSweep3(aabbMin,aabbMax);
		btCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
		btDispatcher*             dispatcher             = new btCollisionDispatcher(collisionConfiguration);

		return reinterpret_cast <dcwCollisionWorld*>(
			new btCollisionWorld(
				dispatcher, 
				broadphasePairCache, 
				collisionConfiguration
			)
		);
	}

	void dcwCollisionWorldDestroy(dcwCollisionWorld *v) {
		delete reinterpret_cast <btCollisionWorld*> (v);
		return;
	}

	void dcwAddColisionObject(
			dcwCollisionWorld* world, 
			dcwCollisionObject* collisionObject) {
		btCollisionWorld*  btColWorld = reinterpret_cast <btCollisionWorld*>(world);
		btCollisionObject* btColObj   = reinterpret_cast <btCollisionObject*>(collisionObject);
		return;
	}

 	void dcwAddColisionObject(
			dcwCollisionWorld* world, 
			dcwCollisionObject* collisionObject) {
		btCollisionWorld*  btColWorld = reinterpret_cast <btCollisionWorld*>(world);
		btCollisionObject* btColObj   = reinterpret_cast <btCollisionObject*>(collisionObject);
		return;
	}

	void dcwSetGravity(
			dcwCollisionWorld* world, 
			dcwVector3* gravity) {
		btCollisionWorld* btColWorld = reinterpret_cast <btCollisionWorld*>(world);
		btVector3*        btGrav     = reinterpret_cast <btVector3*>(gravity);
		btColWorld->setGravity(btGrav);
		return;
	}

#ifdef __cplusplus
}
#endif

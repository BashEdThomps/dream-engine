#ifdef __cplusplus
#include "../BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "../BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#include "../BulletCollision/BroadphaseCollision/btAxisSweep3.h"
#include "../BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "../LinearMath/btVector3.h"
#endif

#include "dcwAxisSweep3.h"
#include "dcwPersistentManifold.h"
#include "dcwCollisionWorld.h"

#ifdef __cplusplus
extern "C" {
#endif

	dcwCollisionWorld* dcwCollisionWorldCreate(
		dcwCollisionDispatcher*           dispatcher,
		dcwAxisSweep3*                    a3PairCache,
		dcwDefaultCollisionConfiguration* collisionConf) {

		btCollisionDispatcher*    btDispatcher    = reinterpret_cast<btCollisionDispatcher*>(dispatcher);
		btAxisSweep3*             bta3PairCache   = reinterpret_cast<btAxisSweep3*>(a3PairCache);
		btDefaultCollisionConfiguration* btCollisionConf = reinterpret_cast<btDefaultCollisionConfiguration*>(collisionConf);

		return reinterpret_cast <dcwCollisionWorld*>(
			new btCollisionWorld(btDispatcher, bta3PairCache, btCollisionConf));
	}

	void dcwCollisionWorldDestroy(dcwCollisionWorld *v) {
		// TODO Extract and delete the ructed variables in create
		delete reinterpret_cast <btCollisionWorld*> (v);
		return;
	}

	void dcwCollisionWorldAddCollisionObject(
			dcwCollisionWorld* world,
			dcwCollisionObject* collisionObject) {
		btCollisionWorld*  btColWorld = reinterpret_cast <btCollisionWorld*>(world);
		btCollisionObject* btColObj   = reinterpret_cast <btCollisionObject*>(collisionObject);
		btColWorld->addCollisionObject(btColObj);
		return;
	}

 	void dcwCollisionWorldRemoveCollisionObject(
			dcwCollisionWorld* world,
			dcwCollisionObject* collisionObject) {
		btCollisionWorld*  btColWorld = reinterpret_cast <btCollisionWorld*>(world);
		btCollisionObject* btColObj   = reinterpret_cast <btCollisionObject*>(collisionObject);
		btColWorld->removeCollisionObject(btColObj);
		return;
	}

	int dcwCollisionWorldGetNumCollisionObjects(
			dcwCollisionWorld* world) {
				int retval = 0;
				btCollisionWorld*  btColWorld = reinterpret_cast <btCollisionWorld*>(world);
				retval = btColWorld->getNumCollisionObjects();
				return retval;
	}


	void dcwCollisionWorldPerformDiscreteCollisionDetection (dcwCollisionWorld* world) {
		btCollisionWorld* btWorld = reinterpret_cast<btCollisionWorld*>(world);
		btWorld->performDiscreteCollisionDetection();
		return;
	}

	dcwCollisionDispatcher* dcwCollisionWorldGetDispatcher(dcwCollisionWorld* world) {
		return reinterpret_cast<dcwCollisionDispatcher*> (
			reinterpret_cast<btCollisionWorld*>(world)->getDispatcher()
		);
	}
#ifdef __cplusplus
}
#endif

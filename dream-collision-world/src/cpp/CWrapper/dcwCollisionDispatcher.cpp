#ifdef __cplusplus
// CPP Only Headers
#include "../BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#include "../BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#endif

#include "dcwPersistentManifold.h"
#include "dcwDefaultCollisionConfiguraion.h"
#include "dcwCollisionDispatcher.h"

#ifdef __cplusplus
extern "C" {
#endif

dcwCollisionDispatcher* dcwCollisionDispatcherCreate(dcwDefaultCollisionConfiguration* config) {
	return reinterpret_cast<dcwCollisionDispatcher*>(
		new btCollisionDispatcher(
			reinterpret_cast<btDefaultCollisionConfiguration*>(config)
		)
	);
}

void dcwCollisionDispatcherDestroy(dcwCollisionDispatcher* obj) {
	delete reinterpret_cast<btCollisionDispatcher*>(obj);
}

int dcwCollisionDispatcherGetNumManifolds(dcwCollisionDispatcher* obj) {
	return reinterpret_cast<btDefaultCollisionConfiguration*>(obj)->getNumManifolds();
}

dcwPersistentManifold* dcwGetManifoldByIndexInternal(dcwCollisionDispatcher* obj , int index) {
	return reinterpret_cast<dcwPersistentManifold*>(
		(reinterpret_cast<btCollisionDispatcher*>obj)->getManifoldByIndexInternal(index)
	);
}
#ifdef __cplusplus
}
#endif

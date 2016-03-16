#include "dcwPersistentManifold.h"

#ifdef __cplusplus
#include "../BulletCollision/NarrowPhaseCollision/btPersistentManifold.h"
#include "../BulletCollision/CollisionDispatch/btCollisionObject.h"
#endif

dcwPersistentManifold* dcwPersistentManifoldCreate() {
	return reinterpret_cast<dcwPersistentManifold*> (
		new btPersistentManifold()
	);
}

void dcwPersistentManifoldDestroy(dcwPersistentManifold* obj) {
	delete reinterpret_cast<btPersistentManifold*>(obj);
	return;
}


const dcwCollisionObject* dcwPersistentManifoldGetBody0(dcwPersistentManifold* obj) {
	const btCollisionObject* colObj = reinterpret_cast<btPersistentManifold*>(obj)->getBody0();
	return reinterpret_cast<const dcwCollisionObject*> (colObj);
}

const dcwCollisionObject* dcwPersistentManifoldGetBody1(dcwPersistentManifold* obj) {
	const btCollisionObject* colObj = reinterpret_cast<btPersistentManifold*>(obj)->getBody1();
	return reinterpret_cast<const dcwCollisionObject*> (colObj);
}

int dcwPersistentManifoldGetNumContacts(dcwPersistentManifold* obj) {
	return (reinterpret_cast<btPersistentManifold*>(obj)->getNumContacts());
}

dcwManifoldPoint* dcwPersistentManifoldGetContactPoint(dcwPersistentManifold* obj, int index) {
	return reinterpret_cast<dcwManifoldPoint*>(
		&reinterpret_cast<btPersistentManifold*>(obj)->getContactPoint(index)
	);
}



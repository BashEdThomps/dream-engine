#include "dcwPersistentManifold.h"

#ifdef __cplusplus
#include "../BulletCollision/NarrowPhaseCollision/btPersistentManifold.h"
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
	return reinterpret_cast<const dcwCollisionObject*> (
		reinterpret_cast<btPersistentManifold*>(obj)->getBody0()
	);
}

const dcwCollisionObject* dcwPersistentManifoldGetBody1(dcwPersistentManifold* obj) {
	return reinterpret_cast<const dcwCollisionObject*> (
		reinterpret_cast<btPersistentManifold*>(obj)->getBody1()
	);
}

int dcwPersistentManifoldGetNumContacts(dcwPersistentManifold* obj) {
	return reinterpret_cast<btPersistentManifold*>(obj)->getNumContacts();
}

dcwManifoldPoint* dcwPersistentManifoldGetContactPoint(dcwPersistentManifold* obj, int index) {
	return reinterpret_cast<dcwManifoldPoint*>(
		&reinterpret_cast<btPersistentManifold*>(obj)->getContactPoint(index)
	);
}



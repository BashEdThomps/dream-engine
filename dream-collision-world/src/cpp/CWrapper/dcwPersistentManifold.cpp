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


dcwCollisionObject* dcwPersistentManifoldGetBody0(dcwPersistentManifold* obj) {
	return reinterpret_cast<dcwCollisionObject*> (
		reinterpret_cast<btPersistentManifold*>(obj)->getdBody0();
	);
}

dcwCollisionObject* dcwPersistentManifoldGetBody1(dcwPersistentManifold* obj) {
	return reinterpret_cast<dcwCollisionObject*> (
		reinterpret_cast<btPersistentManifold*>(obj)->getdBody1();
	);
}

int dcwPersistentManifoldGetNumContacts(dcwPersistentManifold* obj) {
	return reinterpret_cast<btPersistentManifold*>(obj)->getNumContacts();
}

dcwManifoldPoint* dcwPersistentManifoldGetContactPoint(dcwPersistentManifold* obj) {
	return reinterpret_cast<btPersistentManifold*>(obj)->getNumContacts();
}



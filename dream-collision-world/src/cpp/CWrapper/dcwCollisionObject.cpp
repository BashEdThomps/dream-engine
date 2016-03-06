#include "dcwCollisionObject.h"

#ifdef __cplusplus
#include  "../BulletCollision/CollisionDispatch/btCollisionObject.h"
#include "../BulletCollision/CollisionShapes/btBoxShape.h"
extern "C" {
#endif

	dcwCollisionObject* dcwCollisionObjectCreate () {
		return reinterpret_cast <dcwCollisionObject*>(new btCollisionObject());
	}

	void dcwCollisionObjectDestroy (dcwCollisionObject* obj) {
		delete reinterpret_cast<btCollisionObject*>(obj);
		return;
	}

	void dcwCollisionObjectSetCollisionBoxShape(dcwCollisionObject* object, dcwBoxShape* box) {
		btBoxShape* btBox = reinterpret_cast<btBoxShape*>(box);
		btCollisionObject* btObj = reinterpret_cast<btCollisionObject*>(object);
		btObj->setCollisionShape(btBox);
		return;
	}

#ifdef __cplusplus
}
#endif

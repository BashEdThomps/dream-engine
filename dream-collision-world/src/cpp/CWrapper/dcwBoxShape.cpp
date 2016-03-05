#include "dcwBoxShape.h"

#ifdef __cplusplus

#include "../LinearMath/btVector3.h"
#include "../BulletCollision/CollisionShapes/btBoxShape.h"

extern "C" {
#endif
	dcwBoxShape* dcwBoxShapeCreate  (dcwVector3* v) {
		btVector3* vector = reinterpret_cast<btVector3*>(v);
		return reinterpret_cast<dcwBoxShape*>(
			new btBoxShape(*vector)
		);
	}

	void dcwBoxShapeDestroy (dcwBoxShape* box) {
		delete reinterpret_cast<btBoxShape*>(box);
		return;
	}

#ifdef __cplusplus
}
#endif



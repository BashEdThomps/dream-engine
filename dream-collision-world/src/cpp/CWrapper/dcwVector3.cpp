#include "../../cpp/LinearMath/btScalar.h"
#include "../../cpp/LinearMath/btVector3.h"

#include "dcwVector3.h"

#ifdef __cplusplus
extern "C" {
#endif

 dcwVector3* dcwVector3Create  (
		 dcwScalar* x,
		 dcwScalar* y,
		 dcwScalar* z) {
	 btScalar *sX, *sY, *sZ;

	sX = reinterpret_cast< btScalar*>(x);
	sY = reinterpret_cast< btScalar*>(y);
	sZ = reinterpret_cast< btScalar*>(z);

	return reinterpret_cast< dcwVector3*>(
		new btVector3(*sX,*sY,*sZ)
	);
}

void dcwVector3Destroy ( dcwVector3* vector) {
	delete reinterpret_cast< btVector3*>(vector);
	return;
}

const dcwScalar* dcwVector3GetX ( dcwVector3* vec) {
	 const dcwScalar* retval = 0;
	 btVector3* btX = reinterpret_cast< btVector3*>(vec);
	retval = reinterpret_cast<const dcwScalar*>(&btX->getX());
	return retval;
}

const dcwScalar* dcwVector3GetY ( dcwVector3* vec) {
	 const dcwScalar* retval = 0;
	 btVector3* btY = reinterpret_cast< btVector3*>(vec);
	retval = reinterpret_cast<const dcwScalar*>(&btY->getY());
	return retval;
}

const dcwScalar* dcwVector3GetZ ( dcwVector3* vec) {
	 const dcwScalar* retval = 0;
	 btVector3* btZ = reinterpret_cast< btVector3*>(vec);
	retval = reinterpret_cast<const dcwScalar*>(&btZ->getZ());
	return retval;
}

void dcwVector3SetX ( dcwVector3* vec,  dcwScalar* x) {
	 btScalar* btx = reinterpret_cast< btScalar*>(x);
	 btVector3* btVec = reinterpret_cast< btVector3*>(vec);
	btVec->setX(*btx);
	return;
}

void dcwVector3SetY ( dcwVector3* vec,  dcwScalar* y) {
	 btScalar* bty = reinterpret_cast< btScalar*>(y);
	 btVector3* btvec = reinterpret_cast< btVector3*>(vec);
	btvec->setY(*bty);
	return;
}

void dcwVector3SetZ ( dcwVector3* vec,  dcwScalar* z) {
	 btScalar* btz = reinterpret_cast< btScalar*>(z);
	 btVector3* btvec = reinterpret_cast< btVector3*>(vec);
	btvec->setZ(*btz);
	return;
}

#ifdef __cplusplus
}
#endif

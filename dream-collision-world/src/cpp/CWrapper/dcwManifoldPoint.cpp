#include "dcwScalar.h"
#include "dcwManifoldPoint.h"
#include "dcwVector3.h"

#ifdef __cplusplus
#include "../BulletCollision/NarrowPhaseCollision/btManifoldPoint.h"
#include "../LinearMath/btScalar.h"
#endif

dcwManifoldPoint* dcwManifoldPointCreate(void) {
	return reinterpret_cast<dcwManifoldPoint*> (
		new btManifoldPoint()
	);
}

void dcwManifoldPointDestroy(dcwManifoldPoint* obj) {
	delete reinterpret_cast<btManifoldPoint*>(obj);
}

const dcwVector3* dcwManifoldPointGetPositionWorldOnA (dcwManifoldPoint* obj) {
	return reinterpret_cast<const dcwVector3*>(
		&(reinterpret_cast<btManifoldPoint*>(obj))->getPositionWorldOnA()
	);
}

const dcwVector3* dcwManifoldPointGetPositionWorldOnB (dcwManifoldPoint* obj) {
	return reinterpret_cast<const dcwVector3*>(
		&(reinterpret_cast<btManifoldPoint*>(obj))->getPositionWorldOnB()
	);

}
/*
const dcwVector3* dcwManifoldPointGetNormalWorldOnA   (dcwManifoldPoint* obj) {
	return reinterpret_cast<const dcwVector3*>(
		(reinterpret_cast<btManifoldPoint*>(obj))->m_normalWorldOnA
	);

}
*/
dcwVector3* dcwManifoldPointGetNormalWorldOnB  (dcwManifoldPoint* obj) {
	return reinterpret_cast<dcwVector3*>(
		&(reinterpret_cast<btManifoldPoint*>(obj)->m_normalWorldOnB)
	);

}

dcwScalar* dcwManifoldPointGetDistance(dcwManifoldPoint* obj) {
	btScalar scalar = reinterpret_cast<btManifoldPoint*>(obj)->getDistance();
	return reinterpret_cast<dcwScalar*>(&scalar);
}

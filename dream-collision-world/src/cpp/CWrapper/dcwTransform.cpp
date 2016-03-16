#include "../LinearMath/btTransform.h"

#include "dcwVector3.h"
#include "dcwTransform.h"

#ifdef __cplusplus
extern "C" {
#endif

dcwTransform* dcwTransformCreate  () {
    return reinterpret_cast<dcwTransform*>(
        new btTransform()
    );
}

void dcwTransformDestroy (dcwTransform* obj) {
	delete reinterpret_cast<btTransform*>(obj);
}

void dcwTransformSetOrigin (dcwTransform* transform, dcwVector3* origin) {
	btTransform* btTrans = reinterpret_cast <btTransform*>(transform);
	btVector3* btVec     = reinterpret_cast <btVector3*>  (origin);
	btTrans->setOrigin(*btVec);
	return;
}

#ifdef __cplusplus
}
#endif

#include "dcwVector3.h"
#include "dcwAxisSweep3.h"

#ifdef __cplusplus
#include "../LinearMath/btVector3.h"
#include "../BulletCollision/BroadphaseCollision/btAxisSweep3.h"
#endif

dcwAxisSweep3* dcwAxisSweep3Create(dcwVector3 *aabbMin, dcwVector3 *aabbMax) {
    btVector3 *btAabbMin, *btAabbMax;
    btAabbMin = reinterpret_cast<btVector3*>(aabbMin);
    btAabbMax = reinterpret_cast<btVector3*>(aabbMax);
    return reinterpret_cast<dcwAxisSweep3*>(new btAxisSweep3(*btAabbMin,*btAabbMax));
}

void dcwAxisSweep3Destroy(dcwAxisSweep3* obj) {
    delete reinterpret_cast<btAxisSweep3*>(obj);
    obj = 0;
}

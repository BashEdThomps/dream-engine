#include "dcwSortedOverlappingPairCache.h"

#ifdef __cplusplus

#include "../BulletCollision/BroadphaseCollision/btOverlappingPairCache.h"

extern "C" {
#endif

dcwSortedOverlappingPairCache* dcwSortedOverlappingPairCacheCreate() {
	return reinterpret_cast<dcwSortedOverlappingPairCache*>(
		new btSortedOverlappingPairCache()
	);
}

void dcwOverlappingPairCacheDestroy(dcwSortedOverlappingPairCache* obj) {
	delete reinterpret_cast<btSortedOverlappingPairCache*>(obj);
	return;
}

#ifdef __cplusplus
}
#endif


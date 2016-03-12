#include "../../../../unit/src/unit.h"

#include "../../cpp/CWrapper/dcwSortedOverlappingPairCache.h"
#include "testSortedOverlappingPairCache.h"

void testSortedOverlappingPairCache(void) {
	testSortedOverlappingPairCacheCreate();
	testSortedOverlappingPairCacheDestroy();
	return;
}

void testSortedOverlappingPairCacheCreate(void) {
	unitTestHeading("Sorted Overlapping Pair Cache - Create");
	dcwSortedOverlappingPairCache *pairCache = dcwSortedOverlappingPairCacheCreate();
	unitAssertNotNull("Create and Destroy Sorted Overlapping Pair Cache", pairCache);
	dcwSortedOverlappingPairCacheDestroy(pairCache);
	return;

}

void testSortedOverlappingPairCacheDestroy(void) {
	unitTestHeading("Sorted Overlapping Pair Cache - Destroy");

	dcwSortedOverlappingPairCache *pairCache = dcwSortedOverlappingPairCacheCreate();
	unitAssertNotNull("Create and Destroy Sorted Overlapping Pair Cache", pairCache);

	dcwSortedOverlappingPairCacheDestroy(pairCache);
	unitAssertNull("PairCache is destroyed",pairCache);
	return;

}

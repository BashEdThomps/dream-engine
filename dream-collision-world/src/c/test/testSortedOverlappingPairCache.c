#include <unit.h>

#include "../../cpp/CWrapper/dcwSortedOverlappingPairCache.h"
#include "testSortedOverlappingPairCache.h"

void testSortedOverlappingPairCache(void){
	unitTestHeading("Sorted Overlapping Pair Cache");
	dcwSortedOverlappingPairCache *pairCache = dcwSortedOverlappingPairCacheCreate();
	unitAssertNotNull("Create and Destroy Sorted Overlapping Pair Cache", pairCache);
	dcwSortedOverlappingPairCacheDestroy(pairCache);
	return;
}

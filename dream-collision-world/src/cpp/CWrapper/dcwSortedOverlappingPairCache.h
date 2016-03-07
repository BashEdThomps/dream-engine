#ifndef  DCW_SOPCACHE_H
#define  DCW_SOPCACHE_H

struct dcwSortedOverlappingPairCache;
typedef struct dcwSortedOverlappingPairCache dcwSortedOverlappingPairCache;

#ifdef __cplusplus
extern "C" {
#endif

	dcwSortedOverlappingPairCache* 
		dcwSortedOverlappingPairCacheCreate ();

	void 
		dcwSortedOverlappingPairCacheDestroy (dcwSortedOverlappingPairCache*); 

#ifdef __cplusplus
}
#endif

#endif  // DCW_SOPCACHE_H

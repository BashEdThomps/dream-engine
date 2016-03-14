#ifndef DCW_MANIFOLDPOINT_H
#define DCW_MANIFOLDPOINT_H

#include "dcwScalar.h"
#include "dcwVector3.h"

struct dcwManifoldPoint;
typedef struct dcwManifoldPoint dcwManifoldPoint;

#ifdef __cplusplus
extern "C" {
#endif

	dcwManifoldPoint* dcwManifoldPointCreate(void);
	void dcwManifoldPointDestroy(dcwManifoldPoint*);

	const dcwVector3* dcwManifoldPointGetPositionWorldOnA (dcwManifoldPoint*);
	const dcwVector3* dcwManifoldPointGetPositionWorldOnB (dcwManifoldPoint*);

	dcwVector3* dcwManifoldPointGetNormalWorldOnB   (dcwManifoldPoint*);
	dcwScalar* dcwManifoldPointGetDistance          (dcwManifoldPoint* );
	
#ifdef __cplusplus
}
#endif

#endif // DCW_MANIFOLDPOINT_H

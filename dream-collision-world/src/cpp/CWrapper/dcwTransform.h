#ifndef DCWTRANSFORM_H
#define DCWTRANSFORM_H

struct dcwTransform;
typedef struct dcwTransform dcwTransform;

#include "dcwVector3.h"

#ifdef __cplusplus
extern "C" {
#endif

dcwTransform* dcwTransformCreate  ();
void          dcwTransformDestroy (dcwTransform*);

void dcwTransformSetOrigin        (dcwTransform*, dcwVector3*);

#ifdef __cplusplus
}
#endif

#endif  //DCWTRANSFORM_H

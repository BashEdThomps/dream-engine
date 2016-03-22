#ifndef DA_FRAMEDELTA_H
#define DA_FRAMEDELTA_H

#include "daConstants.h"

//! TODO - Document
typedef struct {
    int    drawableID;
    float *positionDelta;
    float *rotationDelta;
    float *scaleDelta;
    float  orbitRadius;
    int    operation;
} daFrameDelta;

//! Create frame delta with drawableID and Operation
daFrameDelta* daFrameDeltaCreate             (int, int);
//! TODO - Document
daFrameDelta* daFrameDeltaComputeFrameDelta  (daFrameDelta*,daFrameDelta*,int,int);
//! TODO - Document
float*        daFrameDeltaComputeDeltaVector (float*, float*);
//! TODO - Document
void          daFrameDeltaPrintDebug         (daFrameDelta*);

#endif // DA_FRAMEDELTA_H

#ifndef DA_FRAMEDELTA_H
#define DA_FRAMEDELTA_H

#include "daConstants.h"

typedef struct {
    int    drawableID;
    float* positionDelta;
    float* rotationDelta;
    float* scaleDelta;
    float  orbitRadius;
    int    operation;
} daFrameDelta;

daFrameDelta* daFrameDeltaCreate             (int, int);
daFrameDelta* daFrameDeltaGetMotionDelta     (daFrameDelta*,daFrameDelta*,int,int);
float*        daFrameDeltaComputeDeltaVector (float*, float*);
void          daFrameDeltaPrintDebug         (daFrameDelta*);

#endif // DA_FRAMEDELTA_H

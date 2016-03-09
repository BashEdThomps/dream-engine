#include <stdlib.h>
#include <stdio.h>

#include "daConstants.h"
#include "daFrameDelta.h"

daFrameDelta* daFrameDeltaCreate(int drawableId, int operation) {
    daFrameDelta* retval = (daFrameDelta*)malloc(sizeof(daFrameDelta));
    retval->operation            = operation;
    retval->drawableID           = drawableId;

    retval->positionDelta        = (float*)malloc(sizeof(float)*DA_CART_ARY_SZ);
    retval->positionDelta [DA_X] = 0;
    retval->positionDelta [DA_Y] = 0;
    retval->positionDelta [DA_Z] = 0;

    retval->rotationDelta        = (float*)malloc(sizeof(float)*DA_CART_ARY_SZ);
    retval->rotationDelta [DA_X] = 0;
    retval->rotationDelta [DA_Y] = 0;
    retval->rotationDelta [DA_Z] = 0;

    retval->orbitRadius          = 0;

    return retval;
}

daFrameDelta* daFrameDeltaGetMotionDelta(
    daFrameDelta* start,
    daFrameDelta* end,
    int steps,
    int stepIndex) {
        daFrameDelta* d = daFrameDeltaCreate(start->drawableID, start->operation);
        float* posDelta = daFrameDeltaComputeDeltaVector(start->positionDelta, end->positionDelta);
        float* rotDelta = daFrameDeltaComputeDeltaVector(start->rotationDelta, end->rotationDelta);

        switch (d->operation) {
            default:
            case DA_OP_LINEAR:
                posDelta [DA_X] /= steps;
                posDelta [DA_Y] /= steps;
                posDelta [DA_Z] /= steps;
                break;
            case DA_OP_ORBIT:
                break;
            case DA_OP_BEZIER:
                break;
        }


        rotDelta[DA_X] /= steps;
        rotDelta[DA_Y] /= steps;
        rotDelta[DA_Z] /= steps;

        d->positionDelta[DA_X] = posDelta[DA_X];
        d->positionDelta[DA_Y] = posDelta[DA_Y];
        d->positionDelta[DA_Z] = posDelta[DA_Z];

        d->rotationDelta[DA_X] = rotDelta[DA_X];
        d->rotationDelta[DA_Y] = rotDelta[DA_Y];
        d->rotationDelta[DA_Z] = rotDelta[DA_Z];

        return d;
    }

float* daFrameDeltaComputeDeltaVector(float* a, float* b) {
    float* retval = (float*)malloc(sizeof(float)*DA_CART_ARY_SZ);
    retval[DA_X] = a[DA_X] - b[DA_X];
    retval[DA_Y] = a[DA_Y] - b[DA_Y];
    retval[DA_Z] = a[DA_Z] - b[DA_Z];
    return retval;
}

void daFrameDeltaPrintDebug(daFrameDelta* obj) {
    fprintf(stdout,"Delta for ID: %d\n"     , obj->drawableID);
    fprintf(stdout,"\tPOS: X:%f Y:%f Z:%f\n", obj->positionDelta[DA_X], obj->positionDelta[DA_Y], obj->positionDelta[DA_Z]);
    fprintf(stdout,"\tROT: X:%f Y:%f Z:%f\n", obj->rotationDelta[DA_X], obj->rotationDelta[DA_Y], obj->rotationDelta[DA_Z]);
}

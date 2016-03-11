#include "../../../unit/src/unit.h"

#include "../daFrame.h"
#include "testFrameDelta.h"

void testFrameDelta(void) {
    unitTestHeading("Frame Delta");
    testFrameDeltaComputeFrameDelta();
    unitTestFooter("Frame Delta");
    return;
}

void testFrameDeltaComputeFrameDelta(void) {
	daFrame *frame = daFrameCreate(0);

	daFrameDelta *delta1 = daFrameDeltaCreate(0,DA_OP_LINEAR);
	daFrameDelta *delta2 = daFrameDeltaCreate(0,DA_OP_LINEAR);

	daFrameAddFrameDelta(frame,delta1);
	daFrameAddFrameDelta(frame,delta2);

	unitAssertNotNull("Frame Delta Computed for Frame",daFrameDeltaComputeFrameDelta(delta1, delta2,10,5));
	return;
}



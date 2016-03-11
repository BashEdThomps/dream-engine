#include "../../../unit/src/unit.h"

#include "testFrame.h"

#include "../daFrame.h"
#include "../daFrameDelta.h"

void testFrame(void) {
	unitTestHeading("Test daFrame");
	testFrameCreate();
	testFrameAddFrameDelta();
	testFrameGetNextAvailableFrameDeltaIndex();
	testFrameGetIndex();
	testFrameCompareIndicies();
	testFrameGetNumFrameDeltas ();
	unitTestFooter("Test daFrame");
}

void testFrameCreate(void) {
    daFrame *frame = daFrameCreate(0);
    unitAssertNotNull("daFrameCreate", frame);
    return;
}

void testFrameAddFrameDelta(void) {
	daFrame *frame = daFrameCreate(0);

	daFrameDelta *delta1 = daFrameDeltaCreate(0,DA_OP_LINEAR);
	daFrameDelta *delta2 = daFrameDeltaCreate(0,DA_OP_LINEAR);

	daFrameAddFrameDelta(frame,delta1);
	daFrameAddFrameDelta(frame,delta2);

	unitAssertEqualInt("Number of deltas in frame is 2", daFrameGetNumFrameDeltas(frame),2);
    return;
}
void testFrameGetNextAvailableFrameDeltaIndex(void){
	daFrame* frame = daFrameCreate(0);

	unitAssertEqualInt(
		"Get Next Available Frame Delta Index of empty frame is 0",
		daFrameGetNextAvailableFrameDeltaIndex(frame),
		0
	);

	daFrameDelta* delta = daFrameDeltaCreate(0,DA_OP_LINEAR);
	daFrameAddFrameDelta(frame,delta);
	unitAssertEqualInt(
		"Get Next Available Frame Delta Index of frame with 1 delta is 1",
		daFrameGetNextAvailableFrameDeltaIndex(frame),
		1
	);
    return;
}

void testFrameGetIndex(void) {
	unitAssertFail("GetIndex");
    return;
}

void testFrameCompareIndicies(void) {
	unitAssertFail("CompareIndicies");
    return;
}

void testFrameGetNumFrameDeltas(void) {
	unitAssertFail("GetNumFrameDeltas");
	return;
}

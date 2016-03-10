#include "../../../unit/src/unit.h"

#include "testFrame.h"

#include "../daFrame.h"
#include "../daFrameDelta.h"

void testFrame(void) {
    unitTestHeading("Test deFrame");
    testFrameCreate();
    testFrameAddMotionDelta();
    testFrameGetNextAvailableFrameDeltaIndex();
    testFrameGetIndex();
    testFrameCompareIndicies();
    unitTestFooter("Test deFrame");
}

void testFrameCreate(void) {
    daFrame *frame = daFrameCreate(0);
    unitAssertNotNull("daFrameCreate", frame);
    return;
}

void testFrameAddMotionDelta(void) {
	daFrame *frame = daFrameCreate(0);
	daFrameDelta *delta = daFrameDeltaCreate(0,DA_OP_LINEAR);
	daFrameAddMotionDelta(frame,delta);
	unitAssertNotZero("Number of deltas in frame > 0",daFrameGetNumFrameDeltas(frame));
	unitAssertEqual("Frame Delta Found In Frame",daFrameGetMotionDelta(frame,0),delta);
    return;
}

void testFrameGetNextAvailableFrameDeltaIndex(void){
    return;
}

void testFrameGetIndex(void) {
    return;
}

void testFrameCompareIndicies(void) {
    return;
}

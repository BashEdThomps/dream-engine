#include "../../../unit/src/unit.h"
#include "testFrame.h"
#include "../daFrame.h"

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

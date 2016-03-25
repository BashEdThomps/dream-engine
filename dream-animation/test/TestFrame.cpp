#include "../../../unit/src/unit.h"

#include "TestFrame.h"

#include "../src/Frame.h"
#include "../src/FrameDelta.h"

void testFrame(void) {
	unitTestHeading("daFrame");
	testFrameCreate();
	testFrameAddFrameDelta();
	testFrameGetNextAvailableFrameDeltaIndex();
	testFrameCompareIndicies();
	testFrameGetNumFrameDeltas ();
}

void testFrameCreate(void) {
	unitPrintComment("Create Frame");
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

void testFrameCompareIndicies(void) {
	daFrame *frame1, *frame2;

	frame1 = daFrameCreate(100);
	frame2 = daFrameCreate(200);

	int result1 = daFrameCompareIndecies(frame1,frame2);
	int result2 = daFrameCompareIndecies(frame2,frame1);

	unitAssertEqualInt("Frame Index Comparison",result1,100);
	unitAssertEqualInt("Frame Index Comparison",result2,-100);
	return;
}

void testFrameGetNumFrameDeltas(void) {
	daFrame* frame = daFrameCreate(0);

	daFrameDelta *delta1, *delta2;
	delta1 = daFrameDeltaCreate(0,0);
	daFrameAddFrameDelta(frame,delta1);
	unitAssertEqualInt("Get Num Frame Deltas == 1",daFrameGetNumFrameDeltas(frame),1);

	delta2 = daFrameDeltaCreate(0,0);	
	daFrameAddFrameDelta(frame,delta2);
	unitAssertEqualInt("Get Num Frame Deltas == 2",daFrameGetNumFrameDeltas(frame),2);
	return;
}

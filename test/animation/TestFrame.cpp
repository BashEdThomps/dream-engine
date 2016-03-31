#include "TestFrame.h"

#include "../src/Frame.h"
#include "../src/FrameDelta.h"

namespace DreamAnimationTest {

	void TestFrame::run() {
		testHeading("DreamAnimation::Frame");
		testFrameCreate();
		testFrameAddFrameDelta();
		testFrameGetNextAvailableFrameDeltaIndex();
		testFrameCompareIndicies();
		testFrameGetNumFrameDeltas ();
	}

	void TestFrame::testFrameCreate(void) {
		printComment("Create Frame");
		DreamAnimation::Frame *frame = new DreamAnimation::Frame(0);
		assertNotNull("Frame Constructor", frame);
		return;
	}

	void TestFrame::testFrameAddFrameDelta(void) {
		DreamAnimation::Frame *frame = new DreamAnimation::Frame(0);
		DreamAnimation::FrameDelta *delta1 = new DreamAnimation::FrameDelta(0,DA_OP_LINEAR);
		DreamAnimation::FrameDelta *delta2 = new DreamAnimation::FrameDelta(0,DA_OP_LINEAR);
		frame->addFrameDelta(delta1);
		frame->addFrameDelta(delta2);
		assertEqual("Number of deltas in frame is 2", frame->getNumFrameDeltas(),2);
		return;
	}
	void TestFrame::testFrameGetNextAvailableFrameDeltaIndex(void){
		DreamAnimation::Frame* frame = new DreamAnimation::Frame(0);

		assertEqual(
			"Get Next Available Frame Delta Index of empty frame is 0",
			frame->getNextAvailableFrameDeltaIndex(),
			0
		);

		DreamAnimation::FrameDelta* delta = new DreamAnimation::FrameDelta(0,DA_OP_LINEAR);
		frame->addFrameDelta(delta);
		assertEqual(
			"Get Next Available Frame Delta Index of frame with 1 delta is 1",
			frame->getNextAvailableFrameDeltaIndex(),
			1
		);
		return;
	}

	void TestFrame::testFrameCompareIndicies(void) {
		DreamAnimation::Frame *frame1;
		DreamAnimation::Frame *frame2;

		frame1 = new DreamAnimation::Frame(100);
		frame2 = new DreamAnimation::Frame(200);

		int result1 = frame1->compareIndecies(frame2);
		int result2 = frame2->compareIndecies(frame1);

		assertEqual("Frame Index Comparison",result1,100);
		assertEqual("Frame Index Comparison",result2,-100);
		return;
	}

	void TestFrame::testFrameGetNumFrameDeltas(void) {
		DreamAnimation::Frame* frame = new DreamAnimation::Frame(0);

		DreamAnimation::FrameDelta *delta1, *delta2;
		delta1 = new DreamAnimation::FrameDelta(0,0);
		frame->addFrameDelta(delta1);
		assertEqual("Get Num Frame Deltas == 1",frame->getNumFrameDeltas(),1);

		delta2 = new DreamAnimation::FrameDelta(0,0);
		frame->addFrameDelta(delta2);
		assertEqual("Get Num Frame Deltas == 2",frame->getNumFrameDeltas(),2);
		return;
	}
}

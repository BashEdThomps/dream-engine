#ifndef TEST_FRAME_H
#define TEST_FRAME_H

#include "../../dream-unit/src/Unit.h"

namespace DreamAnimationTest {
	class TestFrame : public DreamUnit::Unit {
	public:
		//! Run Method
		void run(void);
		//! Test creating a daFrame.
		void testFrameCreate        (void);
		//! Tests adding a daFrameDelta to a daFrame.
		void testFrameAddFrameDelta (void);
		//! Run all Frame Tests.
		void testFrameGetNextAvailableFrameDeltaIndex(void);
		//! Tests comparing indicies of two daFrames.
		void testFrameCompareIndicies   (void);
		//! Tests counting the number of daFrameDeltas held by the frame.
		void testFrameGetNumFrameDeltas (void);
	};
}

#endif  // TEST_FRAME_H

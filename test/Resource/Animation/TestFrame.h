/*
* Dream::Animation::Test::TestFrame
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TEST_FRAME_H
#define TEST_FRAME_H

#include "../../../src/Unit/Unit.h"

namespace Dream {
	namespace Resource {
		namespace Animation {
			namespace Test {
				class TestFrame : public Dream::Unit::Unit {
				public:
					TestFrame();
					~TestFrame();
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
		}
	}
}

#endif  // TEST_FRAME_H

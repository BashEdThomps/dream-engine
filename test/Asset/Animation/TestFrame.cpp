/*
* Dream::Asset::Animation::Test::TestFrame
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

#include "TestFrame.h"

#include "../../../src/Asset/Instance/Animation/Frame.h"
#include "../../../src/Asset/Instance/Animation/FrameDelta.h"

namespace Dream {
	namespace Asset {
		namespace Animation {
			namespace Test {
				TestFrame::TestFrame() :
				Dream::Unit::Unit("Dream::Asset::Animation:Frame") {

				}

				TestFrame::~TestFrame() {

				}

				void TestFrame::run() {
					header();
					testFrameCreate();
					testFrameAddFrameDelta();
					testFrameCompareIndicies();
					testFrameGetNumFrameDeltas ();
				}

				void TestFrame::testFrameCreate(void) {
					comment("Create Frame");
                    Dream::Asset::Instance::Animation::Frame *frame =
                        new Dream::Asset::Instance::Animation::Frame(0);
					assertNotNull("Frame Constructor", frame);
					return;
				}

				void TestFrame::testFrameAddFrameDelta(void) {
                    Dream::Asset::Instance::Animation::Frame *frame =
                        new Dream::Asset::Instance::Animation::Frame(0);
                    
					Dream::Asset::Instance::Animation::FrameDelta *delta1 =
                    
                        new Dream::Asset::Instance::Animation::FrameDelta(0,DA_OP_LINEAR);
                    
					Dream::Asset::Instance::Animation::FrameDelta *delta2 =
                        new Dream::Asset::Instance::Animation::FrameDelta(0,DA_OP_LINEAR);
                    
					frame->addFrameDelta(delta1);
					frame->addFrameDelta(delta2);
					assertEqual("Number of deltas in frame is 2", frame->getNumFrameDeltas(),2);
					return;
				}

				void TestFrame::testFrameCompareIndicies(void) {
					Dream::Asset::Instance::Animation::Frame *frame1;
					Dream::Asset::Instance::Animation::Frame *frame2;

					frame1 = new Dream::Asset::Instance::Animation::Frame(100);
					frame2 = new Dream::Asset::Instance::Animation::Frame(200);

					int result1 = frame1->compareIndecies(frame2);
					int result2 = frame2->compareIndecies(frame1);

					assertEqual("Frame Index Comparison",result1,100);
					assertEqual("Frame Index Comparison",result2,-100);
					return;
				}

				void TestFrame::testFrameGetNumFrameDeltas(void) {
					Dream::Asset::Instance::Animation::Frame* frame =
                        new Dream::Asset::Instance::Animation::Frame(0);

					Dream::Asset::Instance::Animation::FrameDelta *delta1, *delta2;
					delta1 = new Dream::Asset::Instance::Animation::FrameDelta(0,0);
					frame->addFrameDelta(delta1);
					assertEqual("Get Num Frame Deltas == 1",frame->getNumFrameDeltas(),1);

					delta2 = new Dream::Asset::Instance::Animation::FrameDelta(0,0);
					frame->addFrameDelta(delta2);
					assertEqual("Get Num Frame Deltas == 2",frame->getNumFrameDeltas(),2);
					return;
				}
			}
		}
	}
}

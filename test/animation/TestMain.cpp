/*
* Dream::Animation::Test::TestMain
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
#include "TestFrameDelta.h"
#include "TestKeyFrame.h"
#include "TestAnimation.h"

using namespace Dream::Animation::Test;

int main(void) {
	int result = 0;
	TestFrame *testFrame = new TestFrame();
	testFrame->run();
	result += testFrame->getResult();
	delete testFrame;

	TestFrameDelta *testFrameDelta = new TestFrameDelta();
	testFrameDelta->testComputeFrameDelta();
	result += testFrameDelta->getResult();
	delete testFrameDelta;

	TestKeyFrame *testKeyFrame = new TestKeyFrame();
	result += testKeyFrame->getResult();
	delete testKeyFrame;

	TestAnimation *testAnimation = new TestAnimation();
	result += testAnimation->getResult();
	delete testAnimation;
}

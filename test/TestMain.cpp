/*
* Main
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

//#include "DreamTest.h"
//#include "SceneTest.h"
//#include "ProjectTest.h"

#include "Unit/UnitTest.h"
#include "Animation/TestFrame.h"
#include "Animation/TestFrameDelta.h"
#include "Animation/TestKeyFrame.h"
#include "Animation/TestAnimation.h"

int main(void) {
	int result = 0;

	// Unit
	Dream::Unit::Test::UnitTest *unit = new Dream::Unit::Test::UnitTest();
    unit->run();
    result += unit->getResult();

	// Animation
	Dream::Animation::Test::TestFrame testFrame;
	testFrame.run();
	result += testFrame.getResult();

	Dream::Animation::Test::TestFrameDelta testFrameDelta;
	testFrameDelta.testComputeFrameDelta();
	result += testFrameDelta.getResult();

	Dream::Animation::Test::TestKeyFrame testKeyFrame;
	result += testKeyFrame.getResult();

	Dream::Animation::Test::TestAnimation testAnimation;
	result += testAnimation.getResult();

	/* Dream
	DreamTest::DreamTest dreamTest;
	dreamTest.run();
	result += dreamTest.getResult();

	DreamTest::ProjectTest projectTest;
	projectTest.run();
	result += projectTest.getResult();

	DreamTest::SceneTest sceneTest;
	sceneTest.run();
	result += sceneTest.getResult();
    */
	return result;
}

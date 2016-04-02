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

#include "Unit/UnitTest.h"

#include "DreamTest.h"
#include "ProjectTest.h"

#include "Scene/SceneTest.h"

#include "Resource/TestResource.h"
#include "Resource/Animation/TestFrame.h"
#include "Resource/Animation/TestFrameDelta.h"
#include "Resource/Animation/TestKeyFrame.h"
#include "Resource/Animation/TestAnimation.h"

int testUnit() {
	int result = 0;
	Dream::Unit::Test::UnitTest unit;
    unit.run();
    result += unit.getResult();
	return result;

}

int testResource() {
	int result = 0;
	Dream::Resource::Test::TestResource testResource;
	testResource.run();
	result += testResource.getResult();
	return result;
}

int testResourceAnimation() {
	int result = 0;

	Dream::Resource::Animation::Test::TestFrame testFrame;
	testFrame.run();
	result += testFrame.getResult();

	Dream::Resource::Animation::Test::TestFrameDelta testFrameDelta;
	testFrameDelta.testComputeFrameDelta();
	result += testFrameDelta.getResult();

	Dream::Resource::Animation::Test::TestKeyFrame testKeyFrame;
	result += testKeyFrame.getResult();

	Dream::Resource::Animation::Test::TestAnimation testAnimation;
	result += testAnimation.getResult();

	return result;
}

int testScene() {
	int result = 0;

	Dream::Scene::Test::SceneTest sceneTest;
	sceneTest.run();
	result += sceneTest.getResult();

	return result;
}

int testDream() {
	int result = 0;

	Dream::Test::DreamTest dreamTest;
	dreamTest.run();
	result += dreamTest.getResult();

	Dream::Test::ProjectTest projectTest;
	projectTest.run();
	result += projectTest.getResult();

	return result;
}

int main(int argc, char** argv) {
	int result = 0;
	result += testUnit();
	result += testResourceAnimation();
	result += testResource();
	result += testScene();
	result += testDream();
	return result;
}

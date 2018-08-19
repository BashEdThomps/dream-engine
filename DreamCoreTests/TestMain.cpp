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
#include "Util/FileReaderTest.h"
#include "Util/UUIDTest.h"

#include "DreamTest.h"
#include "ProjectTest.h"
#include "ArgumentParserTest.h"

#include "Scene/SceneTest.h"
#include "Scene/TestSceneObject.h"

#include "Asset/TestAsset.h"
#include "Asset/Path/TestFrame.h"
#include "Asset/Path/TestKeyFrame.h"
#include "Asset/Path/TestPath.h"

int testUnit() {
	int result = 0;

	Dream::Unit::Test::UnitTest unit;
    unit.run();
    result += unit.getResult();

	return result;
}

int testAsset() {
	int result = 0;

	Dream::Asset::Test::TestAsset testAsset;
	testAsset.run();
	result += testAsset.getResult();

	return result;
}

int testAssetPath() {
	int result = 0;

	Dream::Asset::Path::Test::TestFrame testFrame;
	testFrame.run();
	result += testFrame.getResult();

	Dream::Asset::Path::Test::TestKeyFrame testKeyFrame;
	testKeyFrame.run();
	result += testKeyFrame.getResult();

	Dream::Asset::Path::Test::TestPath testPath;
	testPath.run();
	result += testPath.getResult();

	return result;
}

int testScene() {
	int result = 0;

	Dream::Scene::Test::SceneTest sceneTest;
	sceneTest.run();
	result += sceneTest.getResult();

	Dream::Scene::Test::TestSceneObject testSceneObject;
	testSceneObject.run();
	result += testSceneObject.getResult();

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

	Dream::Test::ArgumentParserTest argumentParserTest;
	argumentParserTest.run();
	result += argumentParserTest.getResult();

	return result;
}

int testUtil() {
	int res = 0;

	Dream::Util::Test::FileReaderTest fileReaderTest;
	fileReaderTest.run();
	res += fileReaderTest.getResult();

	Dream::Util::Test::UUIDTest uuidTest;
	uuidTest.run();
	res += uuidTest.getResult();
	return res;
}

int main(int argc, char** argv) {
	int result = 0;
	result += testUnit();
	result += testUtil();
	result += testAssetPath();
	result += testAsset();
	result += testScene();
	result += testDream();
	return result;
}

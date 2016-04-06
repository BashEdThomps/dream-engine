/*
* DreamSceneTest
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

#include "SceneTest.h"
#include "../../src/Scene/Scene.h"

namespace Dream {
	namespace Scene {
		namespace Test {
			SceneTest::SceneTest() : Dream::Unit::Unit("Dream::Scene::Scene"){
			}

			SceneTest::~SceneTest() {
			}

			void SceneTest::run(void) {
				header();
				testSceneCanStoreUUID();
				testSceneCanStoreName();
				testSceneHasScenegraph();
			}

			void SceneTest::testSceneCanStoreUUID() {
				Scene p;
				std::string uuid = "12ab-34cd-56ef-78af";
				p.setUUID(uuid);
				assertTrue("Stores UUID",p.getUUID().compare(uuid) == 0);
			}

			void SceneTest::testSceneCanStoreName(void) {
				Dream::Scene::Scene s;
				std::string name = "TestScene";
				s.setName(name);
				assertTrue("Scene has name",s.getName().compare(name) == 0);
			}

			void SceneTest::testSceneHasScenegraph() {
				Scene s;
				assertNotNull("Scene has Scenegraph",s.getScenegraph());
			}
		} // End of Test
	} // End of Scene
} // End of Dream

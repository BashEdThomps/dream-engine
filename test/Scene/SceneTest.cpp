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

namespace Dream {
	namespace Scene {
		namespace Test {
			SceneTest::SceneTest() {
			}

			SceneTest::~SceneTest() {
			}

			void SceneTest::run(void) {
				testSceneCanStoreName();
				testSceneCanStoreMultipleSceneObjects();
				testSceneCanStoreMultipleAnimationResources();
				testSceneCanStoreMultipleSoundEffectResources();
				testSceneCanStoreMultipleMusicResources();
			}

			void SceneTest::testSceneCanStoreName(void) {
				assertInconclusive("SceneCanStoreName: Test Not Implemented");
			}

			void SceneTest::testSceneCanStoreMultipleSceneObjects(void) {
				assertInconclusive("SceneCanStoreMultipleSceneObjects: Test Not Implemented");
			}

			void SceneTest::testSceneCanStoreMultipleAnimationResources(void) {
				assertInconclusive("SceneCanStoreMultipleAnimation: Test Not Implemented");
			}

			void SceneTest::testSceneCanStoreMultipleSoundEffectResources(void) {
				assertInconclusive("SceneCanStoreMultipleSoundEffects: Test Not Implemented");
			}

			void SceneTest::testSceneCanStoreMultipleMusicResources(void) {
				assertInconclusive("SceneCanStoreMultipleAudioTracks: Test Not Implemented");
			}
		} // End of Test
	} // End of Scene
} // End of Dream

/*
* DreamTest
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

#include "DreamTest.h"
#include "../src/Dream.h"

#define TEST_PROJECT "test/Ash's Test Project.json"

namespace Dream {
	namespace Test {
		DreamTest::DreamTest() : Unit("Dream::Dream") {
		}

		DreamTest::~DreamTest() {
		}

		void DreamTest::run() {
			header();
			testCanHoldProject();
			testCanLoadProjectFromJSON();
		}

		void DreamTest::testCanHoldProject() {
			Dream dream;
			dream.setProject(new Project());
			assertNotNull("Dream can hold Project",dream.getProject());
		}

		void DreamTest::testCanLoadProjectFromJSON() {
			Dream dream;

			dream.loadProjectFromJsonFile(TEST_PROJECT);

			std::string expectedUUID        = "bd94-d5c9-50b0-2c7b";
			std::string expectedName        = "Ash's Test Project";
			std::string expectedAuthor      = "Ashley Thompson";
			std::string expectedDescription = "A Test Dream project.";

			assertZero("Project Got UUID from JSON",       dream.getProject()->getUUID().compare(expectedUUID));
			assertZero("Project Got Name from JSON",       dream.getProject()->getName().compare(expectedName));
			assertZero("Project Got Author from JSON",     dream.getProject()->getAuthor().compare(expectedAuthor));
			assertZero("Project Got Description from JSON",dream.getProject()->getDescription().compare(expectedDescription));

			std::string sceneName = "First Scene";
			Scene::Scene* firstScene = dream.getProject()->getSceneByName(sceneName);

			assertTrue("Got first scene from project by name", firstScene != NULL);
			assertFalse("Project Scene has animation enabled", firstScene->isAnimationEnabled());
			assertFalse("Project Scene has audio enabled",      firstScene->isAudioEnabled());
			assertFalse("Project Scene has physics enabled",   firstScene->isPhysicsEnabled());
			assertTrue("Project Scene has collision enabled",  firstScene->isCollisionEnabled());
			assertTrue("Project Scene has input enabled",      firstScene->isInputEnabled());
		};
	} // End of Test
} // End of Dream

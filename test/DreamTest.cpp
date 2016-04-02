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
			assertInconclusive("Dream can load project from JSON");
		};
	} // End of Test
} // End of Dream

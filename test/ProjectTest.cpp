/*
* ProjectTest
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

#include "ProjectTest.h"
#include "../src/Project.h"

namespace Dream {
	namespace Test {
		ProjectTest::ProjectTest(void) {
		}

		ProjectTest::~ProjectTest(void) {
		}

		void ProjectTest::run(void) {

		}

		void ProjectTest::testProjectCanStoreName() {
			assertInconclusive("Project Can Store Name");
		}

		void ProjectTest::testProjectCanStoreAuthor(void) {
			assertInconclusive("ProjectCanStoreName: Test Not Implemented");
		}

		void ProjectTest::testProjectCanStoreDescription(void) {
			assertInconclusive("ProjectCanStoreName: Test Not Implemented");
		}

		void ProjectTest::testProjectCanStoreMultipleScenes() {
			assertInconclusive("Project Can Hold Multiple Secnes");
		}

		void ProjectTest::testProjectCanStoreMultipleResources() {
			assertInconclusive("Project can hold multiple resources");
		}
	} // End of Test
} // End of Dream

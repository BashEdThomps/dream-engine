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

namespace DreamTest {

	DreamTest::DreamTest() {
	}

	DreamTest::~DreamTest() {
	}

	void DreamTest::run() {
		testCanHoldScene();
	}

	void DreamTest::testCanHoldScene() {
		Dream::Dream dream;
		dream.setScene(new Dream::Scene());
		assertNotNull("Dream can hold Scene object",dream.getScene());
	}

} // End of DreamTest

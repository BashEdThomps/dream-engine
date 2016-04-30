/*
* UUIDTest
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

#include "UUIDTest.h"
#include "../../src/Util/UUID.h"

namespace Dream {
	namespace Util {
		namespace Test {

			UUIDTest::UUIDTest(void) : Dream::Unit::Unit("Dream::Util::UUID") {
			}

			UUIDTest::~UUIDTest(void) {
			}

			void UUIDTest::run() {
					testGenerateUUID();
			}

			void UUIDTest::testGenerateUUID() {
				assertInconclusive("No Tests To Run, but check this uuid I just made...");
				std::string uuid = Dream::Util::UUID::generateUUID();
				comment(uuid);
			}

		} // End of Test
	} // End of Util
} // End of Dream

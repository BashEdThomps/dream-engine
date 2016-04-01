/*
* Dream::Animation::Test::TestFrameDelta
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

#ifndef TESTFRAMEDELTA_H
#define TESTFRAMEDELTA_H

#include "../../../src/Unit/Unit.h"

namespace Dream {
	namespace Resource {
		namespace Animation {
			namespace Test {
				class TestFrameDelta : public Dream::Unit::Unit {
				public:
					TestFrameDelta(void);
					~TestFrameDelta(void);
					void run(void);
					void testComputeFrameDelta();
				//protected:
				//private:
				}; // End of TestFrameDelta
			} // End of Test
		} // End of Animation
	} // End of Resource
} // End of Dream

#endif // End of TESTFRAMEDELTA_H

/*
* Dream::Path::Test::TestPath
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

#ifndef TESTPATH_H
#define TESTPATH_H

#include "../../../src/Unit/Unit.h"

namespace Dream  {
	namespace Asset {
		namespace Path {
			namespace Test {
				class TestPath : public Dream::Unit::Unit{
				public:
					TestPath();
					~TestPath();
					void run();
				//protected:
				//private:
				}; // End of TestPath
			} // End of Test
		} // End of Path
	} // End of Asset
} // End of Dream

#endif // End of TESTPATH_H

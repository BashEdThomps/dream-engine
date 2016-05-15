/*
* ArgumentParserTest
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

#ifndef ARGUMENTPARSERTEST_H
#define ARGUMENTPARSERTEST_H

#include <iostream>
#include "../src/Unit/Unit.h"
#include "../src/Main/ArgumentParser.h"

namespace Dream {
	namespace Test {
		class ArgumentParserTest : public Dream::Unit::Unit {
		public:
			ArgumentParserTest(void);
			~ArgumentParserTest(void);
			void run(void);
			void testCanGetDirectoryFromArgs(void);
			void testCanGetProjectUUIDFromDirectoryArg(void);
			void testCanGetProjectFilePathFromDirectoryArg(void);
		};
	} // End of ArgumentParserTest
} // End of Dream

#endif // End of ARGUMENTPARSERTEST_H

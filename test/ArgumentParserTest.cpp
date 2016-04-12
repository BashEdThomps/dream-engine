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

#include "ArgumentParserTest.h"

namespace Dream {
	namespace Test {
		ArgumentParserTest::ArgumentParserTest(void) : Dream::Unit::Unit("Dream::ArgumentParser") {

		}

		ArgumentParserTest::~ArgumentParserTest(void) {

		}

		void ArgumentParserTest::run(void) {
			header();
			testCanGetDirectoryFromArgs();
			testCanGetProjectUUIDFromDirectoryArg();
			testCanGetProjectFilePathFromDirectoryArg();
		}

		void ArgumentParserTest::testCanGetDirectoryFromArgs(void) {
			int argc = 3;
			const char* argv[argc];
			std::string dir = "/home/user/.dreamtool/0123-4567-89ab-cdef";
			argv[0]   = std::string("Dream").c_str();
			argv[1]   = std::string(PROJECT_DIRECTORY_ARG).c_str();
			argv[2]   = dir.c_str();
			ArgumentParser ap(argc,argv);
			assertZero("testCanGetDirectoryFromArgs",ap.getProjectDirectory().compare(dir));
		}

		void ArgumentParserTest::testCanGetProjectUUIDFromDirectoryArg(void) {
			int argc = 3;
			const char* argv[argc];
			std::string dir = "/home/user/.dreamtool/0123-4567-89ab-cdef";
			std::string projUUID = "0123-4567-89ab-cdef";
			argv[0]   = std::string("Dream").c_str();
			argv[1]   = std::string(PROJECT_DIRECTORY_ARG).c_str();
			argv[2]   = dir.c_str();
			ArgumentParser ap(argc,argv);
			assertZero("testCanGetProjectUUIDFromDirectoryArg",ap.getProjectUUID().compare(projUUID));
		}

		void ArgumentParserTest::testCanGetProjectFilePathFromDirectoryArg() {
			int argc = 3;
			const char* argv[argc];
			std::string dir = "/home/user/.dreamtool/0123-4567-89ab-cdef";
			std::string projFilePath = "/home/user/.dreamtool/0123-4567-89ab-cdef/0123-4567-89ab-cdef.json";
			argv[0]   = std::string("Dream").c_str();
			argv[1]   = std::string(PROJECT_DIRECTORY_ARG).c_str();
			argv[2]   = dir.c_str();
			ArgumentParser ap(argc,argv);
			//`comment(ap.getProjectFilePath());
			assertZero("testCanGetProjectFilePathFromDirectoryArg",ap.getProjectFilePath().compare(projFilePath));
		}
	} // End of Test
} // End of Dream

/*
* JSONFileReaderTest
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

#include "FileReaderTest.h"

#include "../../src/Util/FileReader.h"

#include <iostream>
#include <string>

namespace Dream {
	namespace Util {
		namespace Test {
			FileReaderTest::FileReaderTest(void) : Dream::Unit::Unit("Dream::Util::FileReader") {
			}

			FileReaderTest::~FileReaderTest(void) {
			}

			void FileReaderTest::run() {
				header();
				testReadIntoBinaryStream();
				testReadIntoStringStream();
			}

			void FileReaderTest::testReadIntoStringStream() {
				FileReader testFileReader(TEST_JSON);
				comment(testFileReader.getPath());
				assertTrue("Read into string stream", testFileReader.readIntoStringStream());
				std::string stringContent = testFileReader.getContentsAsString();
				assertNotZero("Read > 0 length from test.json",stringContent.length());
				comment(stringContent);
			}

			void FileReaderTest::testReadIntoBinaryStream() {
				assertInconclusive("Read file as binary stream");
			}
		} // End of Test
	} // End of Util
} // End of Dream

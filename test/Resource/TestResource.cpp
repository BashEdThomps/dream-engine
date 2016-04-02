/*
* TestResource
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

#include "TestResource.h"
#include "../../src/Resource/Resource.h"

namespace Dream {
	namespace Resource {
		namespace Test {
			TestResource::TestResource(void) : Dream::Unit::Unit("Dream::Resource::Resource") {
			}

			TestResource::~TestResource(void) {
			}

			void TestResource::run() {
				header();
				testResourceCanStoreName();
			}

			void TestResource::testResourceCanStoreName() {
				Dream::Resource::Resource r;
				std::string name = "TestResource";
				r.setName(name);
				assertZero("Resource can store name",name.compare(r.getName()));
			}
		}
	}
} // End of Dream

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

#ifndef TESTRESOURCE_H
#define TESTRESOURCE_H

#include "../../src/Unit/Unit.h"
#include "../../src/Resource/Resource.h"
#include "TestResourceImpl.h"

namespace Dream {
	namespace Resource {
		namespace Test {
			class TestResource : public Dream::Unit::Unit {
			public:
				TestResource(void);
				~TestResource(void);
				void run();
				void testResourceCanStoreUUID();
				void testResourceCanStoreName();
			}; // End of TestResource
		} // End of Test
	} // End of Resource
} // End of Dream

#endif // End of TESTRESOURCE_H

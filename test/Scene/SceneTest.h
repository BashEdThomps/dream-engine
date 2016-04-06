/*
* DreamSceneTest
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

#ifndef DREAMSCENETEST_H
#define DREAMSCENETEST_H

#include "../../src/Unit/Unit.h"

namespace Dream {
	namespace Scene {
		namespace Test {
			class SceneTest : public Dream::Unit::Unit {
			public:
				SceneTest(void);
				~SceneTest(void);
				void run(void);
				void testSceneCanStoreUUID(void);
				void testSceneCanStoreName(void);
				void testSceneHasScenegraph(void);
			}; // End of SceneTest
		} // End of Test
	} // End of Scene
} // End of Dream

#endif // End of DREAMSCENETEST_H

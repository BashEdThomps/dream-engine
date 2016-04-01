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
	namespace Test {
		class SceneTest : public DreamUnit::Unit {
		public:
			SceneTest(void);
			~SceneTest(void);
			void run(void);
			void testSceneCanStoreName(void);
			void testSceneCanStoreMultipleSceneObjects(void);
			void testSceneCanStoreMultipleAnimations(void);
			void testSceneCanStoreMultipleSoundEffects(void);
			void testSceneCanStoreMultiplAudioTracks(void);
		//protected:
		//private:
		}; // End of SceneTest
	} // End of Test
} // End of Dream

#endif // End of DREAMSCENETEST_H

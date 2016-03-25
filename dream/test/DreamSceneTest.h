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

#include "../../dream-unit/src/Unit.h"

namespace DreamTest {
	class SceneTest : public DreamUnit::Unit {
	public:
		SceneTest(void);
		~SceneTest(void);
		void run(void);
	//protected:
	//private:
	}; // End of DreamSceneTest
} // End of DreamTest

#endif // End of DREAMSCENETEST_H

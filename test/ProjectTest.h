/*
* ProjectTest
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

#ifndef PROJECTTEST_H
#define PROJECTTEST_H

#include "../../dream-unit/src/Unit.h"

namespace DreamTest {
	class ProjectTest : public DreamUnit::Unit {
	public:
		ProjectTest(void);
		~ProjectTest(void);
		void run(void);
		void testProjectCanHoldName();
		void testProjectCanHoldAuthor();
		void testProjectCanHoldDescription();
		void testProjectCanHoldMultipleScenes();
		void testProjectCanHoldMultipleResources();
	//protected:
	//private:
	}; // End of ProjectTest
} // End of DreamTest

#endif // End of PROJECTTEST_H

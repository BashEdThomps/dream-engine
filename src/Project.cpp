/*
* Project
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

#include "Project.h"

namespace Dream {

	Project::Project(void) {
	}

	Project::~Project(void) {
	}

	void Project::addScene(Dream::Scene::Scene *scene) {

	}

	int  Project::getNextAvailableSceneIndex() {
		int retval = -1;
		for (int i=0;i<D_MAX_SCENES;i++) {
			if (mScenes[i]==NULL) {
				retval = i;
				break;
			}
		}
		return retval;
	}
} // End of Dream

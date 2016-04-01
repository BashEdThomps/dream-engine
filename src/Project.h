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

#ifndef PROJECT_H
#define PROJECT_H

#include "Scene/Scene.h"

#define D_MAX_SCENES 64

namespace Dream {
	class Project {
	private:
		Dream::Scene::Scene* mScenes[D_MAX_SCENES];
	public:
		Project(void);
		~Project(void);
		void addScene(Dream::Scene::Scene*);
		int  getNextAvailableSceneIndex();
	//protected:
	//private:
	}; // End of Project
} // End of Dream

#endif // End of PROJECT_H

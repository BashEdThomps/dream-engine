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
#include "Resource/Resource.h"

#define D_MAX_SCENES 64
#define D_MAX_RESOURCES 512

namespace Dream {
	class Project {
	private:
		std::string mName;
		std::string mDescription;
		std::string mAuthor;
		Dream::Scene::Scene* mScenes[D_MAX_SCENES];
		Dream::Resource::Resource* mResources[D_MAX_RESOURCES];
	public:
		Project(void);
		~Project(void);

		void setName(std::string);
		std::string getName();

		void setDescription(std::string);
		std::string getDescription(void);

		void setAuthor(std::string);
		std::string getAuthor();

		void addScene(Dream::Scene::Scene*);
		int  getNextAvailableSceneIndex();
		Dream::Scene::Scene* getSceneAtIndex(int);
		void removeScene(Dream::Scene::Scene*);

		void addResource(Dream::Resource::Resource*);
		int getNextAvailableResourceIndex();
		Dream::Resource::Resource* getResourceAtIndex(int);
		void removeResource(Dream::Resource::Resource*);
		int getNumberOfResources();
	//protected:
	//private:
	}; // End of Project
} // End of Dream

#endif // End of PROJECT_H

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
#include "Resource/Model/ModelResource.h"

namespace Dream {

	Project::Project(void) {

	}

	Project::Project(nlohmann::json jsonProject) {
		mUUID        = jsonProject [PROJECT_JSON_UUID];
		mName        = jsonProject [PROJECT_JSON_NAME];
		mAuthor      = jsonProject [PROJECT_JSON_AUTHOR];
		mDescription = jsonProject [PROJECT_JSON_DESCRIPTION];
		loadResourcesFromJson(jsonProject[PROJECT_JSON_RESOURCE_ARRAY]);
		loadScenesFromJson(jsonProject[PROJECT_JSON_SCENE_ARRAY]);
	}

	Project::~Project(void) {
	}

	void Project::setName(std::string name) {
		mName = name;
	}

	void Project::setUUID(std::string uuid) {
		mUUID = uuid;
	}

	std::string Project::getUUID() {
		return mUUID;
	}

	void Project::loadScenesFromJson(nlohmann::json jsonSceneArray) {
		for (nlohmann::json::iterator it = jsonSceneArray.begin(); it != jsonSceneArray.end(); ++it) {
			addScene(new Dream::Scene::Scene((*it)));
		}
	}

 	void Project::loadResourcesFromJson(nlohmann::json jsonResourceArray) {
		for (nlohmann::json::iterator it = jsonResourceArray.begin();
		     it != jsonResourceArray.end(); ++it) {
			Dream::Resource::Resource* resource = NULL;

			std::string resourceType = (*it)[RESOURCE_JSON_TYPE];

			if (resourceType.compare(RESOURCE_TYPE_MODEL) == 0) {
				resource = new Dream::Resource::Model::ModelResource((*it));
			} else if (resourceType.compare(RESOURCE_TYPE_ANIMATION) == 0) {

			} else if (resourceType.compare(RESOURCE_TYPE_MUSIC) == 0) {

			} else if (resourceType.compare(RESOURCE_TYPE_SOUND_EFFECT) == 0) {

			}

			if (resource != NULL) {
				addResource(resource);
			}
		}
	}

	std::string Project::getName() {
		return mName;
	}

	void Project::setDescription(std::string description) {
		mDescription = description;
	}

	std::string Project::getDescription(void) {
		return mDescription;
	}

	void Project::setAuthor(std::string author) {
		mAuthor = author;
	}

	std::string Project::getAuthor() {
		return mAuthor;
	}

	void Project::addScene(Dream::Scene::Scene *scene) {
		mScenes.push_back(scene);
	}

	void Project::addResource(Dream::Resource::Resource* resource) {
		mResources.push_back(resource);
	}

	void Project::removeResource(Dream::Resource::Resource*) {

	}

	int Project::getNumberOfResources() {
		return mResources.size();
	}

	int Project::getNumberOfScenes() {
		return mScenes.size();
	}


	Dream::Scene::Scene* Project::getSceneByName(std::string name) {
		Dream::Scene::Scene* retval = NULL;
		for(std::vector<Dream::Scene::Scene*>::iterator it = mScenes.begin();
	        it != mScenes.end(); ++it) {
				if ((*it)->getName().compare(name) == 0) {
					retval = (*it);
					break;
				}
			}
		return retval;
	}
} // End of Dream

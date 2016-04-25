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
#include "Resource/Model/Model.h"
#include "Resource/Model/Wavefront/ObjModel.h"
#include "Resource/Script/JavaScript/JavaScript.h"

namespace Dream {

	Project::Project(void) {

	}

	Project::Project(nlohmann::json jsonProject) {
		mUUID        = jsonProject [PROJECT_JSON_UUID];
		mName        = jsonProject [PROJECT_JSON_NAME];
		mAuthor      = jsonProject [PROJECT_JSON_AUTHOR];
		mDescription = jsonProject [PROJECT_JSON_DESCRIPTION];


		mOpenALEnabled  = (
			jsonProject [PROJECT_JSON_OPENAL_ENABLED].is_null() ?
			false : (bool) jsonProject [PROJECT_JSON_OPENAL_ENABLED]
		);

		mBullet2Enabled = (
			jsonProject [PROJECT_JSON_BULLET2_ENABLED].is_null() ?
			false : (bool) jsonProject [PROJECT_JSON_BULLET2_ENABLED]
		);

		mBullet3Enabled = (
			jsonProject [PROJECT_JSON_BULLET3_ENABLED].is_null() ?
			false : (bool) jsonProject [PROJECT_JSON_BULLET3_ENABLED]
		);

		mOpenGLEnabled  = (
			jsonProject [PROJECT_JSON_OPENGL_ENABLED].is_null() ?
			false : (bool) jsonProject [PROJECT_JSON_OPENGL_ENABLED]
		);

		mVulkanEnabled  = (
			jsonProject [PROJECT_JSON_VULKAN_ENABLED].is_null() ?
			false : (bool) jsonProject [PROJECT_JSON_VULKAN_ENABLED]
		);

		showStatus();

		loadResourcesFromJson(jsonProject[PROJECT_JSON_RESOURCE_ARRAY]);
		loadScenesFromJson(jsonProject[PROJECT_JSON_SCENE_ARRAY]);
	}

	void Project::showStatus() {
		std::cout << "Project: New Project"  << std::endl;
		std::cout << "               UUID: " << mUUID           << std::endl;
		std::cout << "               Name: " << mName           << std::endl;
		std::cout << "             Author: " << mAuthor         << std::endl;
		std::cout << "        Description: " << mDescription    << std::endl;
		std::cout << "     OpenAL Enabled: " << mOpenALEnabled  << std::endl;
		std::cout << "    Bullet2 Enabled: " << mBullet2Enabled << std::endl;
		std::cout << "    Bullet3 Enabled: " << mBullet3Enabled << std::endl;
		std::cout << "     OpenGL Enabled: " << mOpenGLEnabled  << std::endl;
		std::cout << "     Vulkan Enabled: " << mVulkanEnabled  << std::endl;
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
		std::cout << "Project: Loading Scenes from JSON Array" << std::endl;
		for (nlohmann::json::iterator it = jsonSceneArray.begin(); it != jsonSceneArray.end(); ++it) {
			Dream::Scene::Scene *nextScene = new Dream::Scene::Scene((*it));
			nextScene->showStatus();
			addScene(nextScene);
		}
	}

 	void Project::loadResourcesFromJson(nlohmann::json jsonResourceArray) {
		std::cout << "Project: Loading Resources from JSON Array" << std::endl;
		for (nlohmann::json::iterator it = jsonResourceArray.begin();
		     it != jsonResourceArray.end(); ++it) {
			Dream::Resource::Resource* resource = NULL;

			std::string resourceType   = (*it)[RESOURCE_JSON_TYPE];
			std::string resourceFormat = (*it)[RESOURCE_JSON_FORMAT];

			if (resourceType.compare(RESOURCE_TYPE_MODEL) == 0) {
				std::cout << "Project: Creating Model Resource" << std::endl;
				if (resourceFormat.compare(RESOURCE_FORMAT_WAVEFRONT) == 0) {
					std::cout << "Project: Creating Wavefront Model Resource" << std::endl;
					resource = new Dream::Resource::Model::Wavefront::ObjModel((*it));
				}
			} else if (resourceType.compare(RESOURCE_TYPE_ANIMATION) == 0) {
				std::cout << "Project: Creating Animation Resource" << std::endl;
			} else if (resourceType.compare(RESOURCE_TYPE_AUDIO) == 0) {
				std::cout << "Project: Creating Audio Resource" << std::endl;
			} else if (resourceType.compare(RESOURCE_TYPE_SCRIPT) == 0) {
				std::cout << "Project: Creating Script Resource" << std::endl;
				if (resourceFormat.compare(RESOURCE_FORMAT_JAVASCRIPT) == 0) {
					std::cout << "Project: Creating JavaScript Resource" << std::endl;
					resource = new Dream::Resource::Script::JavaScript::JavaScript((*it));
				}
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

	Dream::Scene::Scene* Project::getSceneByUUID(std::string uuid) {
		Dream::Scene::Scene* retval = NULL;
		for(std::vector<Dream::Scene::Scene*>::iterator it = mScenes.begin();
	        it != mScenes.end(); ++it) {
				if ((*it)->getUUID().compare(uuid) == 0) {
					retval = (*it);
					break;
				}
			}
		return retval;
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

	std::string Project::getDirectory() {
		return mDirectory;
	}

	void Project::setDirectory(std::string dir) {
		mDirectory = dir;
	}

	bool Project::isOpenALEnabled() {
		return mOpenALEnabled;
	}

	bool Project::isBullet2Enabled() {
		return mBullet2Enabled;
	}

	bool Project::isBullet3Enabled() {
		return mBullet3Enabled;
	}

	bool Project::isOpenGLEnabled() {
		return mOpenGLEnabled;
	}

	bool Project::isVulkanEnabled() {
		return mVulkanEnabled;
	}
} // End of Dream

/*
* Dream::Project
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

	Project::Project(std::string projectPath, nlohmann::json jsonProject) {
		setProjectPath(projectPath);
		setMetadata(jsonProject);
		setPluginFlags(jsonProject);
		loadAssetsFromJson(jsonProject[PROJECT_ASSET_ARRAY]);
		loadScenesFromJson(jsonProject[PROJECT_SCENE_ARRAY]);
		showStatus();
	}
	
	void Project::setMetadata(nlohmann::json jsonProject) {
		if (jsonProject [PROJECT_UUID].is_null()) {
			std::cerr << "Project: UUID is NULL." << std::endl;
		} else {
			setUUID(jsonProject[PROJECT_UUID]);
		}
		
		if (jsonProject[PROJECT_NAME].is_null()) {
			std::cerr << "Project: Name is NULL." << std::endl;
		} else {
			setName(jsonProject[PROJECT_NAME]);
		}
		
		if (jsonProject[PROJECT_AUTHOR].is_null()) {
			std::cerr << "Project: Author is NULL." << std::endl;
		} else {
			setAuthor(jsonProject[PROJECT_AUTHOR]);
		}
		
		if (jsonProject[PROJECT_DESCRIPTION].is_null()) {
			std::cerr << "Project: Descriptiont is NULL." << std::endl;
		} else {
			setDescription(jsonProject[PROJECT_DESCRIPTION]);
		}
		
		if (jsonProject[PROJECT_STARTUP_SCENE].is_null()) {
			std::cerr << "Project: Startup Scene is NULL." << std::endl;
		} else {
			setStartupSceneUUID(jsonProject[PROJECT_STARTUP_SCENE]);
		}
		
	}

	void Project::setPluginFlags(nlohmann::json jsonProject) {
		mChaiEnabled  = (
			jsonProject [PROJECT_CHAI_ENABLED].is_null() ?
			false : (bool) jsonProject [PROJECT_CHAI_ENABLED]
		);

		mV8Enabled  = (
			jsonProject [PROJECT_V8_ENABLED].is_null() ?
			false : (bool) jsonProject [PROJECT_V8_ENABLED]
		);

		mOpenALEnabled  = (
			jsonProject [PROJECT_OPENAL_ENABLED].is_null() ?
			false : (bool) jsonProject [PROJECT_OPENAL_ENABLED]
		);

		mBullet2Enabled = (
			jsonProject [PROJECT_BULLET2_ENABLED].is_null() ?
			false : (bool) jsonProject [PROJECT_BULLET2_ENABLED]
		);

		mBullet3Enabled = (
			jsonProject [PROJECT_BULLET3_ENABLED].is_null() ?
			false : (bool) jsonProject [PROJECT_BULLET3_ENABLED]
		);

		mOpenGLEnabled  = (
			jsonProject [PROJECT_OPENGL_ENABLED].is_null() ?
			false : (bool) jsonProject [PROJECT_OPENGL_ENABLED]
		);

		mVulkanEnabled  = (
			jsonProject [PROJECT_VULKAN_ENABLED].is_null() ?
			false : (bool) jsonProject [PROJECT_VULKAN_ENABLED]
		);
	}

	void Project::showStatus() {
		std::cout << "Project: New Project"  << std::endl;
		std::cout << "               UUID: " << mUUID                  << std::endl;
		std::cout << "               Name: " << mName                  << std::endl;
		std::cout << "             Author: " << mAuthor                << std::endl;
		std::cout << "        Description: " << mDescription           << std::endl;
		std::cout << "       Chai Enabled: " << mChaiEnabled           << std::endl;
		std::cout << "         v8 Enabled: " << mV8Enabled             << std::endl;
		std::cout << "     OpenAL Enabled: " << mOpenALEnabled         << std::endl;
		std::cout << "    Bullet2 Enabled: " << mBullet2Enabled        << std::endl;
		std::cout << "    Bullet3 Enabled: " << mBullet3Enabled        << std::endl;
		std::cout << "     OpenGL Enabled: " << mOpenGLEnabled         << std::endl;
		std::cout << "     Vulkan Enabled: " << mVulkanEnabled         << std::endl;
		std::cout << "             Scenes: " << getNumberOfScenes()    << std::endl;
		std::cout << "      Startup Scene: " << getStartupSceneUUID()  << std::endl;
		std::cout << "          Assets: " << getNumberOfAssets() << std::endl;
	}

	Project::~Project(void) {}

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
			std::cout << "Project: Creating Scene" << std::endl;
			Dream::Scene::Scene *nextScene = new Dream::Scene::Scene((*it));
			nextScene->showStatus();
			addScene(nextScene);
		}
	}

 	void Project::loadAssetsFromJson(nlohmann::json jsonAssetArray) {
		std::cout << "Project: Loading Assets from JSON Array" << std::endl;
		for (nlohmann::json::iterator it = jsonAssetArray.begin(); it != jsonAssetArray.end(); ++it) {
			addAsset(new Dream::Asset::Asset((*it)));
		}
	}

	void Project::setStartupSceneUUID(std::string sceneUUID) {
		mStartupScene = sceneUUID;
	}
	
	std::string Project::getStartupSceneUUID() {
		return mStartupScene;
	}
	
	Dream::Scene::Scene* Project::getStartupScene() {
		return getSceneByUUID(getStartupSceneUUID());
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

	void Project::addAsset(Dream::Asset::Asset* asset) {
		mAssets.push_back(asset);
	}

	void Project::removeAsset(Dream::Asset::Asset*) {
		std::cout << "Project: Remove Asset is not yet Implemented" << std::endl;
	}

	int Project::getNumberOfAssets() {
		return mAssets.size();
	}

	int Project::getNumberOfScenes() {
		return mScenes.size();
	}

	Dream::Scene::Scene* Project::getSceneByUUID(std::string uuid) {
		Dream::Scene::Scene* retval = NULL;
		for(std::vector<Dream::Scene::Scene*>::iterator it = mScenes.begin(); it != mScenes.end(); ++it) {
				if ((*it)->getUUID().compare(uuid) == 0) {
					retval = (*it);
					break;
				}
			}
		return retval;
	}

	Dream::Scene::Scene* Project::getSceneByName(std::string name) {
		Dream::Scene::Scene* retval = NULL;
		for(std::vector<Dream::Scene::Scene*>::iterator it = mScenes.begin(); it != mScenes.end(); ++it) {
				if ((*it)->getName().compare(name) == 0) {
					retval = (*it);
					break;
				}
			}
		return retval;
	}

	std::string Project::getProjectPath() {
		return mProjectPath;
	}

	void Project::setProjectPath(std::string dir) {
		mProjectPath = dir;
	}

	bool Project::isOpenALEnabled() {
		return mOpenALEnabled;
	}

	bool Project::isChaiEnabled() {
		return mChaiEnabled;
	}

	bool Project::isV8Enabled() {
		return mV8Enabled;
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

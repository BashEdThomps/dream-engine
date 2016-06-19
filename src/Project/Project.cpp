/*
* Dream::Project::Project
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
namespace Project {
	
	Scene::Time *Project::sTime = new Scene::Time();
	
	Scene::Time *Project::getTime() {
		return sTime;
	}
	
	Project::Project(void) {
		mAssetManager = NULL;
		mComponentManager = NULL;
		mDone = false;
	}

	Project::Project(std::string projectPath, nlohmann::json jsonProject) {
		mComponentManager = NULL;
		mAssetManager  = NULL;
		mDone          = false;
		setProjectPath(projectPath);
		setMetadata(jsonProject);
		setComponentFlags(jsonProject);
		loadAssetsDefinitionsFromJson(jsonProject[PROJECT_ASSET_ARRAY]);
		loadScenesFromJson(jsonProject[PROJECT_SCENE_ARRAY]);
		createComponentManager();
		createAssetManager();
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

	void Project::setComponentFlags(nlohmann::json jsonProject) {
		mChaiEnabled  = (
			jsonProject [PROJECT_CHAI_ENABLED].is_null() ?
			false : (bool) jsonProject [PROJECT_CHAI_ENABLED]
		);
		
		mOpenALEnabled  = (
			jsonProject [PROJECT_OPENAL_ENABLED].is_null() ?
			false : (bool) jsonProject [PROJECT_OPENAL_ENABLED]
		);

		mBullet2Enabled = (
			jsonProject [PROJECT_BULLET2_ENABLED].is_null() ?
			false : (bool) jsonProject [PROJECT_BULLET2_ENABLED]
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
		std::cout << "               UUID: " << mUUID << std::endl;
		std::cout << "               Name: " << mName << std::endl;
		std::cout << "             Author: " << mAuthor << std::endl;
		std::cout << "        Description: " << mDescription << std::endl;
		std::cout << "      Startup Scene: " << getStartupSceneUUID() << std::endl;
		std::cout << "       Chai Enabled: " << Util::String::boolToYesNo(mChaiEnabled) << std::endl;
		std::cout << "     OpenAL Enabled: " << Util::String::boolToYesNo(mOpenALEnabled) << std::endl;
		std::cout << "    Bullet2 Enabled: " << Util::String::boolToYesNo(mBullet2Enabled) << std::endl;
		std::cout << "     OpenGL Enabled: " << Util::String::boolToYesNo(mOpenGLEnabled) << std::endl;
		std::cout << "     Vulkan Enabled: " << Util::String::boolToYesNo(mVulkanEnabled) << std::endl;
		std::cout << " Assets Definitions: " << getNumberOfAssetDefinitions() << std::endl;
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

 	void Project::loadAssetsDefinitionsFromJson(nlohmann::json jsonAssetArray) {
		std::cout << "Project: Loading Assets from JSON Array" << std::endl;
		for (nlohmann::json::iterator it = jsonAssetArray.begin(); it != jsonAssetArray.end(); ++it) {
			addAssetDefinition(new Dream::Asset::AssetDefinition((*it)));
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

	void Project::addAssetDefinition(Dream::Asset::AssetDefinition* assetDefinition) {
		mAssetDefinitions.push_back(assetDefinition);
	}

	void Project::removeAssetDefinition(Dream::Asset::AssetDefinition*) {
		std::cout << "Project: Remove Asset is not yet Implemented" << std::endl;
	}

	int Project::getNumberOfAssetDefinitions() {
		return mAssetDefinitions.size();
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

	bool Project::isBullet2Enabled() {
		return mBullet2Enabled;
	}

	bool Project::isOpenGLEnabled() {
		return mOpenGLEnabled;
	}
	
	Dream::Asset::AssetDefinition* Project::getAssetDefinitionByUUID(std::string uuid) {
		Dream::Asset::AssetDefinition* retval = NULL;
		for (std::vector<Dream::Asset::AssetDefinition*>::iterator it = mAssetDefinitions.begin(); it != mAssetDefinitions.end(); it++) {
			if ((*it)->getUUID().compare(uuid) == 0) {
				retval = (*it);
				break;
			}
		}
		return retval;
	}
	

	void Project::setActiveScene(Scene::Scene* scene) {
		mActiveScene = scene;
	}
	
	Scene::Scene* Project::getActiveScene() {
		return mActiveScene;
	}
	
	bool Project::hasActiveScene() {
		return getActiveScene() != NULL;
	}
	
	bool Project::initActiveScene() {
		if (mActiveScene != NULL) {
			if (!mActiveScene->init()) {
				return false;
			}
		}
		
		if (!mAssetManager->createAllAssetInstances()) {
			std::cerr << "Project: Unable to create asset instances." << std::endl;
			return false;
		}
		mComponentManager->populatePhysicsWorld(mAssetManager->getSceneObjectsWithPhysicsObjects());
		return true;
	}
	
	bool Project::isChaiEnabled() {
		return mChaiEnabled;
	}
	
	bool Project::isVulkanEnabled() {
		return mVulkanEnabled;
	}
	
	bool Project::createComponentManager() {
		if (mComponentManager != NULL) {
			std::cout << "Project: Destroying existing ComponentManager." << std::endl;
			delete mComponentManager;
		}
		mComponentManager = new Dream::Components::ComponentManager(this);
		return mComponentManager != NULL;
	}
	
	bool Project::createAssetManager() {
		if (mAssetManager != NULL) {
			std::cout << "Project: Destroying existing Asset Manager." << std::endl;
			delete mAssetManager;
		}
		mAssetManager = new Dream::Asset::AssetManager(this);
		return mAssetManager != NULL;
	}
	
	bool Project::loadScene(Scene::Scene* scene) {
		std::cout << "Project: Loading Scene " << scene->getName() << std::endl;
		setActiveScene(scene);
		if (!hasActiveScene()) {
			std::cerr << "Project: Unable to find active scene. Cannot Continue." << std::endl;
			return false;
		}
		if (!initActiveScene()) {
			std::cerr << "Project: Unable to initialise Current Scene." << std::endl;
			return false;
		}
		return true;
	}
	
	bool Project::run (){
		//mComponentManager->setParallel(true);
		// Create Components
		if(!mComponentManager->createComponents()){
			std::cerr << "Project: Unable to create components." << std::endl;
			return false;
		}
		
		// Init Scene with Asset Instances
		if (!loadScene(getStartupScene())) {
			std::cerr << "Project: Unable to load startup scene." << std::endl;
			return false;
		}
		
		mComponentManager->setActiveScene(mActiveScene);
		std::cout << "Project: Starting Scene - "
		          << getActiveScene()->getName()
		          << " (" << getActiveScene()->getUUID() << ")" << std::endl;
		
		// GameLoop
		if (mComponentManager->isParallel()) {
			mComponentManager->startThreads();
		}
		
		while(!mDone) {
			mDone = mComponentManager->isDone();
			sTime->update();
			mComponentManager->update();
			std::this_thread::yield();
		}
		
		if(mComponentManager->isParallel()) {
			mComponentManager->joinThreads();
		}
		return mDone;
	}
	
	Components::ComponentManager* Project::getComponentManager() {
		return mComponentManager;
	}
	
	Asset::AssetManager* Project::getAssetManager() {
		return mAssetManager;
	}
	
} // End of Project
} // End of Dream

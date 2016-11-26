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

  Project::Project(AssetManager* assetManager) {
    setAssetManager(assetManager);
  }

  Project::Project(AssetManager* assetManager, std::string projectPath, nlohmann::json jsonProject) {
    setAssetManager(assetManager);
    setProjectPath(projectPath);
    setMetadata(jsonProject);
    mAssetManager->loadAssetDefinitionsFromJson(jsonProject[PROJECT_ASSET_ARRAY]);
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

    if (!jsonProject[PROJECT_WINDOW_SIZE].is_null()) {
      nlohmann::json windowSizeJson = jsonProject[PROJECT_WINDOW_SIZE];
      setWindowWidth(windowSizeJson[PROJECT_WIDTH]);
      setWindowHeight(windowSizeJson[PROJECT_HEIGHT]);
    }

  }

  void Project::showStatus() {
    std::cout << "Project: "  << std::endl;
    std::cout << "         UUID: " << getUUID() << std::endl;
    std::cout << "         Name: " << getName() << std::endl;
    std::cout << "       Author: " << getAuthor() << std::endl;
    std::cout << "  Description: " << getDescription() << std::endl;
    std::cout << "Startup Scene: " << getStartupSceneUUID() << std::endl;
    std::cout << " Window Width: " << getWindowWidth() << std::endl;
    std::cout << "Window Height: " << getWindowHeight() << std::endl;
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
      Scene *nextScene = new Scene((*it));
      nextScene->showStatus();
      addScene(nextScene);
    }
  }

  void Project::setStartupSceneUUID(std::string sceneUUID) {
    mStartupScene = sceneUUID;
  }

  std::string Project::getStartupSceneUUID() {
    return mStartupScene;
  }

  Scene* Project::getStartupScene() {
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

  void Project::addScene(Scene *scene) {
    mScenes.push_back(scene);
  }

  size_t Project::getNumberOfScenes() {
    return mScenes.size();
  }

  Scene* Project::getSceneByUUID(std::string uuid) {
    Scene* retval = NULL;
    for(std::vector<Scene*>::iterator it = mScenes.begin(); it != mScenes.end(); ++it) {
      if ((*it)->getUUID().compare(uuid) == 0) {
        retval = (*it);
        break;
      }
    }
    return retval;
  }

  Scene* Project::getSceneByName(std::string name) {
    Scene* retval = NULL;
    for(std::vector<Scene*>::iterator it = mScenes.begin(); it != mScenes.end(); ++it) {
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

  void Project::setActiveScene(Scene* scene) {
    mActiveScene = scene;
  }

  Scene* Project::getActiveScene() {
    return mActiveScene;
  }

  bool Project::hasActiveScene() {
    return getActiveScene() != NULL;
  }

  void Project::setAssetManager(AssetManager* assetManager) {
    mAssetManager = assetManager;
  }

  int Project::getWindowWidth() {
    return mWindowWidth;
  }

  void Project::setWindowWidth(int width) {
    mWindowWidth = width;
  }

  int Project::getWindowHeight() {
    return mWindowHeight;
  }

  void Project::setWindowHeight(int height) {
    mWindowHeight = height;
  }

} // End of Dream

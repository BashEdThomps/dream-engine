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

#include <unistd.h>
#include <vector>
#include "json.hpp"
#include "Scene.h"
#include "AssetDefinition.h"
#include "String.h"
#include "AssetManager.h"

#define PROJECT_UUID            "uuid"
#define PROJECT_NAME            "name"
#define PROJECT_AUTHOR          "author"
#define PROJECT_DESCRIPTION     "description"
#define PROJECT_SCENE_ARRAY     "scenes"
#define PROJECT_ASSET_ARRAY     "assets"
#define PROJECT_STARTUP_SCENE   "startupScene"

#define PROJECT_CHAI_ENABLED    "chai"
#define PROJECT_OPENAL_ENABLED  "openAL"
#define PROJECT_BULLET2_ENABLED "bullet2"
#define PROJECT_OPENGL_ENABLED  "openGL"
#define PROJECT_VULKAN_ENABLED  "vulkan"

namespace Dream {

  class Project {
  // Instance Variables
  private:
    AssetManager *mAssetManager;
    std::string mUUID;
    std::string mName;
    std::string mDescription;
    std::string mAuthor;
    std::string mProjectPath;
    std::string mStartupScene;
    std::vector<Scene*> mScenes;
    Scene *mActiveScene;
  // Instance Methods
  public:
    Project(AssetManager*);
    Project(AssetManager*, std::string, nlohmann::json);
    ~Project();

    void setMetadata(nlohmann::json);
    void loadScenesFromJson(nlohmann::json);

    void setAssetManager(AssetManager*);

    void setUUID(std::string);
    std::string getUUID();

    void setName(std::string);
    std::string getName();

    void setDescription(std::string);
    std::string getDescription(void);

    void setAuthor(std::string);
    std::string getAuthor();

    void setStartupSceneUUID(std::string);
    std::string getStartupSceneUUID();
    Scene* getStartupScene();

    void addScene(Scene*);
    void removeScene(Scene*);
    size_t getNumberOfScenes();

    Scene* getSceneByName(std::string);
    Scene* getSceneByUUID(std::string);

    std::string getProjectPath();
    void setProjectPath(std::string);

    void showStatus();

    void setActiveScene(Scene*);
    Scene *getActiveScene();
    bool hasActiveScene();

  }; // End of Project
} // End of Dream

#endif // End of PROJECT_H

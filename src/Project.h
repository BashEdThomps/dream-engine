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

#define PROJECT_WINDOW_SIZE  "windowSize"
#define PROJECT_WIDTH "width"
#define PROJECT_HEIGHT "height"

namespace Dream {

  using namespace std;
  using namespace nlohmann;

 //Instance Variables
  class Project {
  private:
    AssetManager *mAssetManager;
    string mUUID;
    string mName;
    string mDescription;
    string mAuthor;
    string mProjectPath;
    string mStartupScene;
    vector<Scene*> mScenes;
    Scene *mActiveScene;
    int mWindowWidth;
    int mWindowHeight;
  // Instance Methods
  public:
    Project(AssetManager*);
    Project(AssetManager*, string, nlohmann::json);
    ~Project();

    void setMetadata(nlohmann::json);
    void loadScenesFromJson(nlohmann::json);

    void setAssetManager(AssetManager*);

    void setUUID(string);
    string getUUID();

    void setName(string);
    string getName();

    void setDescription(string);
    string getDescription(void);

    void setAuthor(string);
    string getAuthor();

    void setStartupSceneUUID(string);
    string getStartupSceneUUID();
    Scene* getStartupScene();

    void addScene(Scene*);
    void removeScene(Scene*);
    size_t getNumberOfScenes();

    Scene* getSceneByName(string);
    Scene* getSceneByUUID(string);

    string getProjectPath();
    void setProjectPath(string);

    void showStatus();

    void setActiveScene(Scene*);
    Scene *getActiveScene();
    bool hasActiveScene();

    int getWindowWidth();
    void setWindowWidth(int);

    int getWindowHeight();
    void setWindowHeight(int);

  }; // End of Project
} // End of Dream

#endif // End of PROJECT_H

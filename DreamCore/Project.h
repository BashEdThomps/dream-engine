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
#include "Lua/ILuaExposable.h"

#include "Scene.h"
#include "AssetDefinition.h"
#include "String.h"
#include "Constants.h"
#include "Components/Audio/IAudioComponent.h"

using namespace std;
using namespace nlohmann;

namespace Dream
{
  class Project : public ILuaExposable
  {
  private:
    string mUuid;
    string mName;
    string mDescription;
    string mAuthor;
    string mProjectPath;
    string mStartupScene;
    vector<Scene*> mScenes;
    vector<AssetDefinition*> mAssetDefinitions;
    Scene *mActiveScene;
    int mWindowWidth;
    int mWindowHeight;
    IAudioComponent* mAudioComponent;

  public:
    Project(string, nlohmann::json,IAudioComponent*);
    ~Project();

    void setMetadata(nlohmann::json);
    void loadScenesFromJson(nlohmann::json);

    void setUuid(string);
    string getUuid();

    void setName(string);
    string getName();

    void setDescription(string);
    string getDescription(void);

    void setAuthor(string);
    string getAuthor();

    void setStartupSceneUuid(string);
    void setStartupSceneName(string);
    string getStartupSceneUuid();
    Scene* getStartupScene();

    void addScene(Scene*);
    void removeScene(Scene*);
    size_t getNumberOfScenes();

    Scene* getSceneByName(string);
    Scene* getSceneByUuid(string);
    vector<Scene*> getSceneList();

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

    void loadAssetDefinitionsFromJson(nlohmann::json);
    void addAssetDefinition(AssetDefinition*);
    void removeAssetDefinition(AssetDefinition*);
    size_t getNumberOfAssetDefinitions();
    AssetDefinition* getAssetDefinitionByUuid(string);
    vector<AssetDefinition*> getAssetDefinitions();

    void exposeLuaApi(lua_State*);

  }; // End of Project

} // End of Dream

#endif // End of PROJECT_H

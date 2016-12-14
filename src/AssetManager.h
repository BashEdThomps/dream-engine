/*
* Dream::AssetManager
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


#ifndef AssetManager_h
#define AssetManager_h

#include <iostream>
#include <vector>
#include <map>

#include "AssetInstance.h"
#include "AssetDefinition.h"
#include "Components/Scripting/LuaScriptInstance.h"
#include "Components/Audio/AudioInstance.h"
#include "Components/Animation/AnimationInstance.h"
#include "Components/Graphics/SpriteInstance.h"
#include "Components/Graphics/ShaderInstance.h"
#include "Components/Graphics/AssimpModelInstance.h"
#include "Components/Graphics/LightInstance.h"
#include "Components/Graphics/FontInstance.h"
#include "Components/Physics/PhysicsObjectInstance.h"
#include "Scene.h"

namespace Dream {

  class AssetManager {
  private:
    map<SceneObject*,LuaScriptInstance*> mLuaScriptMap;
    vector<AssetDefinition*> mAssetDefinitions;
    string mProjectPath;

  public:
    AssetManager();
    ~AssetManager();
    void cleanupScene(Scene*);
    bool createAllAssetInstances(Scene*);
    AssetInstance* createAssetInstanceFromDefinitionUuid(SceneObject*, string);
    AssetInstance* createAssetInstance(SceneObject*, AssetDefinition*);
    AnimationInstance* createAnimationInstance(SceneObject*, AssetDefinition*);
    AudioInstance* createAudioInstance(SceneObject*, AssetDefinition*);
    AssimpModelInstance* createModelInstance(SceneObject*, AssetDefinition*);
    LuaScriptInstance* createScriptInstance(SceneObject*, AssetDefinition*);
    ShaderInstance* createShaderInstance(SceneObject*, AssetDefinition*);
    PhysicsObjectInstance* createPhysicsObjectInstance(SceneObject*, AssetDefinition*);
    LightInstance* createLightInstance(SceneObject*, AssetDefinition*);
    FontInstance* createFontInstance(SceneObject*, AssetDefinition*);
    SpriteInstance* createSpriteInstance(SceneObject*, AssetDefinition*);

    void loadAssetDefinitionsFromJson(nlohmann::json);
    void addAssetDefinition(AssetDefinition*);
    void removeAssetDefinition(AssetDefinition*);
    size_t getNumberOfAssetDefinitions();
    AssetDefinition* getAssetDefinitionByUuid(string);

    void setProjectPath(string);

    map<SceneObject*,LuaScriptInstance*> *getLuaScriptMap();
    void insertIntoLuaScriptMap(SceneObject*,LuaScriptInstance*);
    bool createAssetInstancesForSceneObject(SceneObject*);
    void removeFromLuaScriptMap(vector<SceneObject*>);

  }; // End of AssetManager
} // End of Dream

#endif /* AssetManager_h */

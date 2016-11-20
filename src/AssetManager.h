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

#include "AssetInstance.h"
#include "AssetDefinition.h"

#include "Instances/Script/Chai/ChaiScriptInstance.h"
#include "Instances/Audio/Ogg/OggAudioInstance.h"
#include "Instances/Audio/Wav/WavAudioInstance.h"
#include "Instances/Animation/Dream/AnimationInstance.h"
#include "Instances/Shader/ShaderInstance.h"
#include "Instances/Model/Assimp/AssimpModelInstance.h"
#include "Instances/Physics/Bullet/PhysicsObjectInstance.h"
#include "Instances/Light/LightInstance.h"

namespace Dream {
  class AssetManager {
  private:
    Project::Project                 *mProject;
    std::vector<AssetInstance*>      mAssetInstances;
    std::vector<SceneObject*> mSceneObjectsWithPhysicsObjects;
  public:
    AssetManager(Project::Project* project);
    ~AssetManager();

    bool createAllAssetInstances();
    void destroyAllAssetInstances();
    void addAssetInstance(AssetInstance*);
    AssetInstance* getAssetInstanceByUUID(std::string);

    AssetInstance* createAssetInstanceFromDefinitionUUID(SceneObject*, std::string);
    AssetInstance* createAssetInstance              (SceneObject*, AssetDefinition*);
    AssetInstance* createAnimationAssetInstance     (SceneObject*, AssetDefinition*);
    AssetInstance* createAudioAssetInstance         (SceneObject*, AssetDefinition*);
    AssetInstance* createModelAssetInstance         (SceneObject*, AssetDefinition*);
    AssetInstance* createScriptAssetInstance        (SceneObject*, AssetDefinition*);
    AssetInstance* createShaderAssetInstance        (SceneObject*, AssetDefinition*);
    AssetInstance* createPhysicsObjectAssetInstance (SceneObject*, AssetDefinition*);
    AssetInstance* createLightAssetInstance         (SceneObject*, AssetDefinition*);
    void           showStatus                       ();
    std::vector<SceneObject*> getSceneObjectsWithPhysicsObjects();
  };
} // End of Dream

#endif /* AssetManager_h */

/*
* Dream::Asset::AssetManager
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


#ifndef AssetManager_hpp
#define AssetManager_hpp

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


namespace Dream {
	
// Project Forward Declaration
namespace Project {
	class Project;
}
	
namespace Asset {
	
	class AssetManager {
	private:
		Project::Project            *mProject;
		std::vector<AssetInstance*>  mAssetInstances;
	public:
		AssetManager(Project::Project* project);
		~AssetManager();
		
		bool createAssetInstances();
		void destroyAssetInstances();
		void addAssetInstance(AssetInstance*);
		
		AssetInstance* createAssetInstanceFromUUID  (Dream::Scene::SceneObject*, std::string);
		AssetInstance* createAssetInstance          (Dream::Scene::SceneObject*, AssetDefinition*);
		AssetInstance* createAnimationAssetInstance (Dream::Scene::SceneObject*, AssetDefinition*);
		AssetInstance* createAudioAssetInstance     (Dream::Scene::SceneObject*, AssetDefinition*);
		AssetInstance* createModelAssetInstance     (Dream::Scene::SceneObject*, AssetDefinition*);
		AssetInstance* createScriptAssetInstance    (Dream::Scene::SceneObject*, AssetDefinition*);
		AssetInstance* createShaderAssetInstance    (Dream::Scene::SceneObject*, AssetDefinition*);
		void           showStatus                   ();
	};
	
} // End of Asset
} // End of Dream

#endif /* AssetManager_hpp */

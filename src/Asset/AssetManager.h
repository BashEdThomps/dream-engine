/*
* Dream::Project::AssetManager
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

#include "../Project/Project.h"

#include "AssetInstance.h"
#include "AssetDefinition.h"

#include "Instances/Script/JavaScript/JavaScriptInstance.h"
#include "Instances/Script/ChaiScript/ChaiScriptInstance.h"
#include "Instances/Audio/Ogg/OggAudioInstance.h"
#include "Instances/Audio/Wav/WavAudioInstance.h"
#include "Instances/Animation/Dream/AnimationInstance.h"
#include "Instances/Shader/ShaderInstance.h"
#include "Instances/Model/Assimp/AssimpModelInstance.h"

namespace Dream {
namespace Asset {
	
	class AssetManager {
	private:
		Project::Project* mProject;
		std::vector<Asset::AssetInstance*> mAssetInstances;
	public:
		AssetManager(Project::Project* project);
		~AssetManager();
		
		bool createAssetInstances();
		void destroyAssetInstances();
		void addAssetInstance(Asset::AssetInstance*);
		
		Asset::AssetInstance* createAssetInstanceFromUUID  (Dream::Scene::SceneObject*, std::string);
		Asset::AssetInstance* createAssetInstance          (Dream::Scene::SceneObject*, Asset::AssetDefinition*);
		Asset::AssetInstance* createAnimationAssetInstance (Dream::Scene::SceneObject*, Asset::AssetDefinition*);
		Asset::AssetInstance* createAudioAssetInstance     (Dream::Scene::SceneObject*, Asset::AssetDefinition*);
		Asset::AssetInstance* createModelAssetInstance     (Dream::Scene::SceneObject*, Asset::AssetDefinition*);
		Asset::AssetInstance* createScriptAssetInstance    (Dream::Scene::SceneObject*, Asset::AssetDefinition*);
		Asset::AssetInstance* createShaderAssetInstance    (Dream::Scene::SceneObject*, Asset::AssetDefinition*);
		
		void showStatus();
	};
	
} // End of Asset
} // End of Dream

#endif /* AssetManager_hpp */

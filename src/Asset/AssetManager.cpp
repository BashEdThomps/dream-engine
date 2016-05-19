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


#include "AssetManager.h"

namespace Dream {
namespace Asset {
	
	AssetManager::AssetManager(Project::Project* project) {
		mProject = project;
	}
	
	AssetManager::~AssetManager() {
		destroyAssetInstances();
	}
	
	void AssetManager::destroyAssetInstances() {
		for (std::vector<Asset::AssetInstance*>::iterator instanceIt = mAssetInstances.begin();
			 instanceIt != mAssetInstances.end(); instanceIt++) {
			delete (*instanceIt);
		}
	}
	
	bool AssetManager::createAssetInstances() {
		std::vector<Scene::SceneObject*> scenegraph = mProject->getActiveScene()->getScenegraphVector();
		std::vector<Scene::SceneObject*>::iterator scenegraphIterator;
		
		std::vector<std::string> assetInstanceUUIDVector;
		std::vector<std::string>::iterator assetInstanceUUIDIterator;
		
		for (scenegraphIterator = scenegraph.begin(); scenegraphIterator != scenegraph.end(); scenegraphIterator++) {
			Scene::SceneObject* currentSceneObject = (*scenegraphIterator);
			assetInstanceUUIDVector = currentSceneObject->getAssetInstanceUUIDVector();
			
			for (assetInstanceUUIDIterator = assetInstanceUUIDVector.begin();
				 assetInstanceUUIDIterator != assetInstanceUUIDVector.end(); assetInstanceUUIDIterator++) {
				
				std::string assetDefinitionUUID = *assetInstanceUUIDIterator;
				Asset::AssetInstance* newAsset = createAssetInstanceFromUUID(assetDefinitionUUID);
				if (newAsset == NULL) {
					std::cerr << "Dream: Unable to instanciate asset instance for " << assetDefinitionUUID << std::endl;
					return false;
				} else {
					addAssetInstance(newAsset);
					currentSceneObject->addAssetInstance(newAsset);
				}
			}
		}
		return true;
	}
	
	void AssetManager::addAssetInstance(Asset::AssetInstance* instance) {
		mAssetInstances.push_back(instance);
	}
	
	Asset::AssetInstance* AssetManager::createAssetInstanceFromUUID(std::string uuid) {
		Asset::AssetDefinition* assetDefinition = mProject->getAssetDefinitionByUUID(uuid);
		return createAssetInstance(assetDefinition);
	}
	
	void AssetManager::showStatus() {
		
	}
	
	Asset::AssetInstance* AssetManager::createAssetInstance(Asset::AssetDefinition* definition) {
		Asset::AssetInstance* retval = NULL;
		std::cout << "Dream: Create Asset Intance from definition: " << definition->getName() << " (" << definition->getType() << ")" << std::endl;
		if (definition->isTypeAnimation()) {
			retval = createAnimationAssetInstance(definition);
		} else if (definition->isTypeAudio()) {
			retval = createAudioAssetInstance(definition);
		} else if (definition->isTypeModel()) {
			retval = createModelAssetInstance(definition);
		} else if (definition->isTypeScript()){
			retval = createScriptAssetInstance(definition);
		}
		return retval;
	}
	
	Asset::AssetInstance* AssetManager::createAnimationAssetInstance(Asset::AssetDefinition* definition) {
		std::cout << "Dream: Creating animation asset instance." << std::endl;
		Asset::AssetInstance* retval = NULL;
		if (definition->isAnimationFormatDream()) {
			retval = new Asset::Instances::Animation::DreamAnimation::AnimationInstance(definition);
		}
		return retval;
	}
	
	
	Asset::AssetInstance* AssetManager::createAudioAssetInstance(Asset::AssetDefinition* definition) {
		std::cout << "Dream: Creating audio asset instance." << std::endl;
		Asset::AssetInstance* retval = NULL;
		if (definition->isAudioFormatOgg()) {
			retval = new Asset::Instances::Audio::Ogg::OggAudioInstance(definition);
		} else if (definition->isAudioFormatWav()) {
			retval = new Asset::Instances::Audio::Wav::WavAudioInstance(definition);
		}
		return retval;
	}
	
	Asset::AssetInstance* AssetManager::createModelAssetInstance(Asset::AssetDefinition* definition) {
		std::cout << "Dream: Creating model asset instance." << std::endl;
		Asset::AssetInstance* retval = NULL;
		if (definition->isModelFormatWaveFront()) {
			retval = new Asset::Instances::Model::WaveFront::ObjModelInstance(definition);
		}
		return retval;
	}
	
	Asset::AssetInstance* AssetManager::createScriptAssetInstance(Asset::AssetDefinition* definition) {
		std::cout << "Dream: Creating script asset instance." << std::endl;
		Asset::AssetInstance* retval = NULL;
		if (definition->isScriptFormatJavaScript()) {
			retval = new Instance::Script::JavaScript::JavaScriptInstance(definition);
		} else if (definition->isScriptFormatChaiScript()) {
			retval = new Asset::Instances::Script::ChaiScript::ChaiScriptInstance(definition);
		}
		return retval;
	}
	
} // End of Instance
} // End of Dream
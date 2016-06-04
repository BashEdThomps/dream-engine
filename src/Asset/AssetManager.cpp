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


#include "../Project/Project.h"
#include "AssetManager.h"

namespace Dream {
namespace Asset {
	
	AssetManager::AssetManager(Project::Project* project) {
		mProject = project;
	}
	
	AssetManager::~AssetManager() {
		destroyAllAssetInstances();
	}
	
	void AssetManager::destroyAllAssetInstances() {
		for (std::vector<AssetInstance*>::iterator instanceIt = mAssetInstances.begin();
			 instanceIt != mAssetInstances.end(); instanceIt++) {
			delete (*instanceIt);
		}
	}
	
	bool AssetManager::createAllAssetInstances() {
		Scene::Scene *activeScene = mProject->getActiveScene();
		
		if (!activeScene){
			std::cerr << "AssetManager: Cannot create assets, no active scene." << std::endl;
			return false;
		}
		
		// Other Assets
		std::vector<Scene::SceneObject*> scenegraph = activeScene->getScenegraphVector();
		std::vector<Scene::SceneObject*>::iterator scenegraphIterator;
		
		std::vector<std::string> assetInstanceUUIDsToLoad;
		std::vector<std::string>::iterator assetInstanceUUIDIterator;
		
		for (scenegraphIterator = scenegraph.begin(); scenegraphIterator != scenegraph.end(); scenegraphIterator++) {
			Scene::SceneObject* currentSceneObject = (*scenegraphIterator);
			assetInstanceUUIDsToLoad = currentSceneObject->getAssetInstanceUUIDsToLoad();
			
			for (assetInstanceUUIDIterator = assetInstanceUUIDsToLoad.begin();
				 assetInstanceUUIDIterator != assetInstanceUUIDsToLoad.end(); assetInstanceUUIDIterator++) {
				
				std::string assetDefinitionUUID = *assetInstanceUUIDIterator;
				AssetInstance* newAsset = createAssetInstanceFromDefinitionUUID(currentSceneObject, assetDefinitionUUID);
				if (newAsset == NULL) {
					AssetDefinition* definition = mProject->getAssetDefinitionByUUID(assetDefinitionUUID);
					std::cerr << "AssetManager: Unable to instanciate asset instance for "
					          << definition->getName() << " (" << definition->getUUID() << ")" << std::endl;
					return false;
				} else {
					addAssetInstance(newAsset);
				}
			}
		}
		return true;
	}
	
	void AssetManager::addAssetInstance(AssetInstance* instance) {
		mAssetInstances.push_back(instance);
	}
	
	AssetInstance* AssetManager::createAssetInstanceFromDefinitionUUID(Scene::SceneObject* sceneObject, std::string uuid) {
		AssetDefinition* assetDefinition = mProject->getAssetDefinitionByUUID(uuid);
		return createAssetInstance(sceneObject, assetDefinition);
	}
	
	void AssetManager::showStatus() {}
	
	AssetInstance* AssetManager::createAssetInstance(Scene::SceneObject* sceneObject,AssetDefinition* definition) {
		AssetInstance* retval = NULL;
		std::cout << "AssetManager: Creating Asset Intance from definition: " << definition->getName() << " (" << definition->getType() << ")" << std::endl;
		
		if(definition->isTypeAnimation()) {
			retval = createAnimationAssetInstance(sceneObject, definition);
		} else if (definition->isTypeAudio()) {
			retval = createAudioAssetInstance(sceneObject, definition);
		} else if (definition->isTypeModel()) {
			retval = createModelAssetInstance(sceneObject, definition);
		} else if (definition->isTypeScript()){
			retval = createScriptAssetInstance(sceneObject, definition);
		} else if (definition->isTypeShader()) {
			retval = createShaderAssetInstance(sceneObject, definition);
		} else if (definition->isTypePhysicsObject()) {
			retval = createPhysicsObjectAssetInstance(sceneObject,definition);
		} else if (definition->isTypeLight()) {
			retval = createLightAssetInstance(sceneObject, definition);
		}
		
		if (retval != NULL) {
			std::cout << "AssetManager: Loading Asset Data for " << definition->getName() << std::endl;
			retval->load(mProject->getProjectPath());
		}
		
		return retval;
	}
	
	AssetInstance* AssetManager::createPhysicsObjectAssetInstance(Scene::SceneObject *sceneObject, AssetDefinition* definition) {
		std::cout << "AssetManager: Creating Physics Object Asset Instance." << std::endl;
		AssetInstance* retval = new Instances::Physics::Bullet::PhysicsObjectInstance(definition);
		
		if (sceneObject) {
			sceneObject->setPhysicsObjectAssetInstance(retval);
			mSceneObjectsWithPhysicsObjects.push_back(sceneObject);
		}
		return retval;
	}
	
	AssetInstance* AssetManager::createAnimationAssetInstance(Scene::SceneObject* sceneObject, AssetDefinition* definition) {
		std::cout << "AssetManager: Creating animation asset instance." << std::endl;
		AssetInstance* retval = NULL;
		
		if (definition->isAnimationFormatDream()) {
			retval = new Instances::Animation::Dream::AnimationInstance(definition);
		}
		
		if (sceneObject) {
			sceneObject->setAnimationAssetInstance(retval);
		}
		return retval;
	}
	
	AssetInstance* AssetManager::createAudioAssetInstance(Scene::SceneObject* sceneObject, AssetDefinition* definition) {
		std::cout << "AssetManager: Creating audio asset instance." << std::endl;
		AssetInstance* retval = NULL;
		
		if (definition->isAudioFormatOgg()) {
			retval = new Instances::Audio::Ogg::OggAudioInstance(definition);
		} else if (definition->isAudioFormatWav()) {
			retval = new Instances::Audio::Wav::WavAudioInstance(definition);
		}
		
		if (sceneObject) {
			sceneObject->setAudioAssetInstance(retval);
		}
		return retval;
	}
	
	AssetInstance* AssetManager::createModelAssetInstance(Scene::SceneObject* sceneObject, AssetDefinition* definition) {
		std::cout << "AssetManager: Creating model asset instance." << std::endl;
		AssetInstance* retval = NULL;
		
		if (definition->isModelFormatAssimp()) {
			retval = new Instances::Model::Assimp::AssimpModelInstance(definition);
		}
	
		if (sceneObject) {
			sceneObject->setModelAssetInstance(retval);
		}
		
		return retval;
	}
	
	AssetInstance* AssetManager::createScriptAssetInstance(Scene::SceneObject* sceneObject, AssetDefinition* definition) {
		std::cout << "AssetManager: Creating script asset instance." << std::endl;
		AssetInstance* retval = NULL;
		
		if (definition->isScriptFormatChai()) {
			std::cout << "AssetManager: Creating Chai Script asset instance." << std::endl;
			Instances::Script::Chai::ChaiScriptInstance* newScript = new Instances::Script::Chai::ChaiScriptInstance(definition);
			retval = newScript;
		}
	
		if (sceneObject) {
			sceneObject->setScriptAssetInstance(retval);
		}
		
		return retval;
	}
	
	AssetInstance* AssetManager::createShaderAssetInstance(Scene::SceneObject* sceneObject, AssetDefinition* definition) {
		std::cout << "AssetManager: Creating Shader asset instance." << std::endl;
		AssetInstance* retval = NULL;
		retval = new Instances::Shader::ShaderInstance(definition);
		if (sceneObject) {
			sceneObject->setShaderAssetInstance(retval);
		}
		return retval;
	}
	
	std::vector<Scene::SceneObject*> AssetManager::getSceneObjectsWithPhysicsObjects() {
		return mSceneObjectsWithPhysicsObjects;
	}
	
	AssetInstance* AssetManager::createLightAssetInstance(Scene::SceneObject *sceneObject, AssetDefinition* definition) {
		std::cout << "AssetManager: Creating Light Asset instance." << std::endl;
		AssetInstance* retval = NULL;
		retval = new Instances::Light::LightInstance(definition);
		
		if (sceneObject) {
			sceneObject->setLightAssetInstance(retval);
		}
		return retval;
	}
	
} // End of Instance
} // End of Dream
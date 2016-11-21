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


#include "AssetManager.h"

namespace Dream {
  AssetManager::AssetManager() {}

  AssetManager::~AssetManager() {
    destroyAllAssetInstances();
  }

  void AssetManager::destroyAllAssetInstances() {
    for (std::vector<AssetInstance*>::iterator instanceIt = mAssetInstances.begin();
       instanceIt != mAssetInstances.end(); instanceIt++) {
      delete (*instanceIt);
    }
  }

  bool AssetManager::createAllAssetInstances(Scene* activeScene) {

    if (!activeScene){
      std::cerr << "AssetManager: Cannot create assets, no active scene." << std::endl;
      return false;
    }

    // Other Assets
    std::vector<SceneObject*> scenegraph = activeScene->getScenegraphVector();
    std::vector<SceneObject*>::iterator scenegraphIterator;

    std::vector<std::string> assetInstanceUUIDsToLoad;
    std::vector<std::string>::iterator assetInstanceUUIDIterator;

    for (scenegraphIterator = scenegraph.begin(); scenegraphIterator != scenegraph.end(); scenegraphIterator++) {
      SceneObject* currentSceneObject = (*scenegraphIterator);
      assetInstanceUUIDsToLoad = currentSceneObject->getAssetInstanceUUIDsToLoad();

      for (assetInstanceUUIDIterator = assetInstanceUUIDsToLoad.begin();
         assetInstanceUUIDIterator != assetInstanceUUIDsToLoad.end(); assetInstanceUUIDIterator++) {

        std::string assetDefinitionUUID = *assetInstanceUUIDIterator;
        AssetInstance* newAsset = createAssetInstanceFromDefinitionUUID(currentSceneObject, assetDefinitionUUID);
        if (newAsset == NULL) {
          AssetDefinition* definition = getAssetDefinitionByUUID(assetDefinitionUUID);
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

  AssetInstance* AssetManager::createAssetInstanceFromDefinitionUUID(SceneObject* sceneObject, std::string uuid) {
    AssetDefinition* assetDefinition = getAssetDefinitionByUUID(uuid);
    return createAssetInstance(sceneObject, assetDefinition);
  }

  void AssetManager::showStatus() {}

  AssetInstance* AssetManager::createAssetInstance(SceneObject* sceneObject,AssetDefinition* definition) {
    AssetInstance* retval = NULL;
    std::cout << "AssetManager: Creating Asset Intance of: (" << definition->getType() << ") " << definition->getName() << ", for SceneObject: " << sceneObject->getNameUUIDString() << std::endl;
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
      retval->load(mProjectPath);
    }

    return retval;
  }

  void AssetManager::setProjectPath(std::string projectPath) {
    mProjectPath = projectPath;
  }

  AssetInstance* AssetManager::createPhysicsObjectAssetInstance(SceneObject *sceneObject, AssetDefinition* definition) {
    std::cout << "AssetManager: Creating Physics Object Asset Instance." << std::endl;
    AssetInstance* retval = new Components::Physics::PhysicsObjectInstance(definition);

    if (sceneObject && retval) {
      sceneObject->setPhysicsObjectAssetInstance(retval);
      mSceneObjectsWithPhysicsObjects.push_back(sceneObject);
    }
    return retval;
  }

  AssetInstance* AssetManager::createAnimationAssetInstance(SceneObject* sceneObject, AssetDefinition* definition) {
    std::cout << "AssetManager: Creating animation asset instance." << std::endl;
    AssetInstance* retval = NULL;

    if (definition->isAnimationFormatDream()) {
      retval = new Components::Animation::AnimationInstance(definition);
    }

    if (sceneObject && retval) {
      sceneObject->setAnimationAssetInstance(retval);
    }
    return retval;
  }

  AssetInstance* AssetManager::createAudioAssetInstance(SceneObject* sceneObject, AssetDefinition* definition) {
    std::cout << "AssetManager: Creating audio asset instance." << std::endl;
    AssetInstance* retval = NULL;

    if (definition->isAudioFormatOgg()) {
      retval = new Components::Audio::OggAudioInstance(definition);
    } else if (definition->isAudioFormatWav()) {
      retval = new Components::Audio::WavAudioInstance(definition);
    }

    if (sceneObject && retval) {
      sceneObject->setAudioAssetInstance(retval);
    }
    return retval;
  }

  AssetInstance* AssetManager::createModelAssetInstance(SceneObject* sceneObject, AssetDefinition* definition) {
    std::cout << "AssetManager: Creating model asset instance." << std::endl;
    AssetInstance* retval = NULL;

    if (definition->isModelFormatAssimp()) {
      retval = new Components::Video::AssimpModelInstance(definition);
    }

    if (sceneObject && retval) {
      sceneObject->setModelAssetInstance(retval);
    }

    return retval;
  }

  AssetInstance* AssetManager::createScriptAssetInstance(SceneObject* sceneObject, AssetDefinition* definition) {
    std::cout << "AssetManager: Creating script asset instance." << std::endl;
    AssetInstance* retval = NULL;

    if (definition->isScriptFormatChai()) {
      std::cout << "AssetManager: Creating Chai Script asset instance." << std::endl;
      Components::Scripting::ChaiScriptInstance* newScript = new Components::Scripting::ChaiScriptInstance(definition);
      retval = newScript;
    }

    if (sceneObject && retval) {
      sceneObject->setScriptAssetInstance(retval);
    }

    return retval;
  }

  AssetInstance* AssetManager::createShaderAssetInstance(SceneObject* sceneObject, AssetDefinition* definition) {
    std::cout << "AssetManager: Creating Shader asset instance." << std::endl;
    AssetInstance* retval = NULL;
    retval = new Components::Video::ShaderInstance(definition);
    if (sceneObject && retval) {
      sceneObject->setShaderAssetInstance(retval);
    }
    return retval;
  }

  std::vector<SceneObject*> AssetManager::getSceneObjectsWithPhysicsObjects() {
    return mSceneObjectsWithPhysicsObjects;
  }

  AssetInstance* AssetManager::createLightAssetInstance(SceneObject *sceneObject, AssetDefinition* definition) {
    std::cout << "AssetManager: Creating Light Asset instance." << std::endl;
    AssetInstance* retval = NULL;
    retval = new Components::Video::LightInstance(definition);

    if (sceneObject && retval) {
      sceneObject->setLightAssetInstance(retval);
    }
    return retval;
  }

  AssetInstance* AssetManager::getAssetInstanceByUUID(std::string uuid) {
    std::vector<AssetInstance*>::iterator assetIt;
    AssetInstance* retval = NULL;
    for (assetIt = mAssetInstances.begin(); assetIt != mAssetInstances.end(); assetIt++) {
      if ((*assetIt)->getUUID().compare(uuid) == 0) {
        retval = (*assetIt);
        break;
      }
    }
    return retval;
  }

  void AssetManager::addAssetDefinition(AssetDefinition* assetDefinition) {
    mAssetDefinitions.push_back(assetDefinition);
  }

  void AssetManager::removeAssetDefinition(AssetDefinition*) {
    std::cout << "AssetManager: Remove Asset is not yet Implemented" << std::endl;
  }

  size_t AssetManager::getNumberOfAssetDefinitions() {
    return mAssetDefinitions.size();
  }

  void AssetManager::loadAssetDefinitionsFromJson(nlohmann::json jsonAssetArray) {
    std::cout << "AssetManager: Loading Assets from JSON Array" << std::endl;
    for (nlohmann::json::iterator it = jsonAssetArray.begin(); it != jsonAssetArray.end(); ++it) {
      addAssetDefinition(new AssetDefinition((*it)));
    }
  }

  AssetDefinition* AssetManager::getAssetDefinitionByUUID(std::string uuid) {
    AssetDefinition* retval = NULL;
    for (std::vector<AssetDefinition*>::iterator it = mAssetDefinitions.begin(); it != mAssetDefinitions.end(); it++) {
      if ((*it)->getUUID().compare(uuid) == 0) {
        retval = (*it);
        break;
      }
    }
    return retval;
  }



} // End of Dream

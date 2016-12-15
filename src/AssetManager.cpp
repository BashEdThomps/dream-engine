/*
* AssetManager
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

  AssetManager::AssetManager() {

  }

  AssetManager::~AssetManager() {
  }

  bool AssetManager::createAllAssetInstances(Scene* activeScene) {
    if (activeScene == nullptr){
      cerr << "AssetManager: Cannot create assets, no active scene." << endl;
      return false;
    }
    vector<SceneObject*> scenegraph = activeScene->getScenegraphVector();
    vector<SceneObject*>::iterator sgIt;
    for (sgIt = scenegraph.begin(); sgIt != scenegraph.end(); sgIt++) {
      SceneObject* sceneObj = (*sgIt);
      if (!sceneObj->getLoadedFlag()) {
        if (!createAssetInstancesForSceneObject(sceneObj)) {
          return false;
        }
      }
    }
    return true;
  }

  bool AssetManager::createAssetInstancesForSceneObject(SceneObject* currentSO) {
    vector<string> aiUuidsToLoad;
    vector<string>::iterator aiUuidIt;
    aiUuidsToLoad = currentSO->getAssetDefUuidsToLoad();
    for (aiUuidIt = aiUuidsToLoad.begin(); aiUuidIt != aiUuidsToLoad.end(); aiUuidIt++) {
      string aDefUuid = *aiUuidIt;
      AssetInstance* newAsset = createAssetInstanceFromDefinitionUuid(currentSO, aDefUuid);
      if (newAsset == nullptr) {
        AssetDefinition* definition = getAssetDefinitionByUuid(aDefUuid);
        cerr << "AssetManager: Unable to instanciate asset instance for "
             << definition->getName() << " (" << definition->getUuid() << ")" << endl;
        return false;
      }
    }
    currentSO->setLoadedFlag(true);
    return currentSO->getLoadedFlag();
  }

  AssetInstance* AssetManager::createAssetInstanceFromDefinitionUuid(SceneObject* sceneObject, string uuid) {
    AssetDefinition* assetDefinition = getAssetDefinitionByUuid(uuid);
    return createAssetInstance(sceneObject, assetDefinition);
  }

  AssetInstance* AssetManager::createAssetInstance(SceneObject* sceneObject,AssetDefinition* definition) {
    AssetInstance* retval = nullptr;

    if (DEBUG) {
      cout << "AssetManager: Creating Asset Intance of: ("
           << definition->getType() << ") " << definition->getName()
           << ", for SceneObject: " << sceneObject->getNameUuidString()
           << endl;
    }

    if(definition->isTypeAnimation()) {
      retval = createAnimationInstance(sceneObject, definition);
    }
    else if (definition->isTypeAudio()) {
      retval = createAudioInstance(sceneObject, definition);
    }
    else if (definition->isTypeModel()) {
      retval = createModelInstance(sceneObject, definition);
    }
    else if (definition->isTypeScript()){
      retval = createScriptInstance(sceneObject, definition);
    }
    else if (definition->isTypeShader()) {
      retval = createShaderInstance(sceneObject, definition);
    }
    else if (definition->isTypePhysicsObject()) {
      retval = createPhysicsObjectInstance(sceneObject,definition);
    }
    else if (definition->isTypeLight()) {
      retval = createLightInstance(sceneObject, definition);
    }
    else if (definition->isTypeSprite()) {
      retval = createSpriteInstance(sceneObject, definition);
    }
    else if (definition->isTypeFont()) {
      retval = createFontInstance(sceneObject,definition);
    }

    if (retval != nullptr) {

      if (DEBUG) {
        cout << "AssetManager:Info:Loading Asset Data for " << definition->getName() << endl;
      }
      if (!retval->load(mProjectPath)) {
        cerr << "AssetManager:Error:Failed to create instance of " << definition->getName() << endl;
        return nullptr;
      }
    }
    return retval;
  }

  void AssetManager::setProjectPath(string projectPath) {
    mProjectPath = projectPath;
  }

  PhysicsObjectInstance* AssetManager::createPhysicsObjectInstance(SceneObject *sceneObject, AssetDefinition* definition) {

    if (DEBUG) {
      cout << "AssetManager: Creating Physics Object Asset Instance." << endl;
    }
    PhysicsObjectInstance* retval = new PhysicsObjectInstance(definition,sceneObject->getTransform());
    sceneObject->setPhysicsObjectInstance(retval);
    return retval;
  }

  AnimationInstance* AssetManager::createAnimationInstance(SceneObject* sceneObject, AssetDefinition* definition) {

    if (DEBUG) {
      cout << "AssetManager: Creating Animation asset instance." << endl;
    }
    AnimationInstance* retval = new AnimationInstance(definition,sceneObject->getTransform());
    sceneObject->setAnimationInstance(retval);
    return retval;
  }

  AudioInstance* AssetManager::createAudioInstance(SceneObject* sceneObject, AssetDefinition* definition) {

    if (DEBUG) {
      cout << "AssetManager: Creating Audio asset instance." << endl;
    }
    AudioInstance* retval = new AudioInstance(definition,sceneObject->getTransform());
    sceneObject->setAudioInstance(retval);
    return retval;
  }

  AssimpModelInstance* AssetManager::createModelInstance(SceneObject* sceneObject, AssetDefinition* definition) {

    if (DEBUG) {
      cout << "AssetManager: Creating Model asset instance." << endl;
    }
    AssimpModelInstance* retval = nullptr;
    retval = new AssimpModelInstance(definition,sceneObject->getTransform());
    sceneObject->setModelInstance(retval);
    return retval;
  }

  LuaScriptInstance* AssetManager::createScriptInstance(SceneObject* sceneObject, AssetDefinition* definition) {

    if (DEBUG) {
      cout << "AssetManager: Creating Script asset instance." << endl;
    }
    LuaScriptInstance* retval = new LuaScriptInstance(definition, sceneObject->getTransform());
    sceneObject->setScriptInstance(retval);
    insertIntoLuaScriptMap(sceneObject,retval);
    return retval;
  }

  ShaderInstance* AssetManager::createShaderInstance(SceneObject* sceneObject, AssetDefinition* definition) {

    if (DEBUG) {
      cout << "AssetManager: Creating Shader asset instance." << endl;
    }
    ShaderInstance* retval = new ShaderInstance(definition,sceneObject->getTransform());
    sceneObject->setShaderInstance(retval);
    return retval;
  }

  LightInstance* AssetManager::createLightInstance(SceneObject *sceneObject, AssetDefinition* definition) {

    if (DEBUG) {
      cout << "AssetManager: Creating Light Asset instance." << endl;
    }
    LightInstance* retval = new LightInstance(definition,sceneObject->getTransform());
    sceneObject->setLightInstance(retval);
    return retval;
  }

  SpriteInstance* AssetManager::createSpriteInstance(SceneObject *sceneObject, AssetDefinition* definition) {

    if (DEBUG) {
      cout << "AssetManager: Creating Sprite Asset instance." << endl;
    }
    SpriteInstance* retval = new SpriteInstance(definition,sceneObject->getTransform());
    sceneObject->setSpriteInstance(retval);
    return retval;
  }

  FontInstance* AssetManager::createFontInstance(SceneObject *sceneObject, AssetDefinition* definition) {

    if (DEBUG) {
      cout << "AssetManager: Creating Font Asset instance." << endl;
    }
    FontInstance* retval = new FontInstance(definition,sceneObject->getTransform());
    sceneObject->setFontInstance(retval);
    return retval;
  }

  void AssetManager::addAssetDefinition(AssetDefinition* assetDefinition) {
    mAssetDefinitions.push_back(assetDefinition);
  }

  void AssetManager::removeAssetDefinition(AssetDefinition* assetDef) {
    for (vector<AssetDefinition*>::iterator it=mAssetDefinitions.begin(); it!=mAssetDefinitions.end(); it++) {
      if ((*it) == assetDef) {

        if (DEBUG) {
          cout << "AssetManager: Removing AssetDefinition " << (*it)->getUuid() << endl;
        }
        mAssetDefinitions.erase(it);
        break;
      }
    }
  }

  size_t AssetManager::getNumberOfAssetDefinitions() {
    return mAssetDefinitions.size();
  }

  void AssetManager::loadAssetDefinitionsFromJson(nlohmann::json jsonAssetArray) {

    if (DEBUG) {
      cout << "AssetManager: Loading Assets from JSON Array" << endl;
    }
    for (nlohmann::json::iterator it = jsonAssetArray.begin(); it != jsonAssetArray.end(); ++it) {
      addAssetDefinition(new AssetDefinition((*it)));
    }
  }

  AssetDefinition* AssetManager::getAssetDefinitionByUuid(string uuid) {
    AssetDefinition* retval = nullptr;
    vector<AssetDefinition*>::iterator it;
    for (it = mAssetDefinitions.begin(); it != mAssetDefinitions.end(); it++) {
      if ((*it)->getUuid().compare(uuid) == 0) {
        retval = (*it);
        break;
      }
    }
    return retval;
  }

  map<SceneObject*,LuaScriptInstance*>* AssetManager::getLuaScriptMap() {
    return &mLuaScriptMap;
  }

  void AssetManager::insertIntoLuaScriptMap(SceneObject* sceneObject,LuaScriptInstance* script) {
    mLuaScriptMap.insert(pair<SceneObject*,LuaScriptInstance*>(sceneObject,script));
  }

  void AssetManager::removeFromLuaScriptMap(vector<SceneObject*> objects) {
    for (vector<SceneObject*>::iterator it=objects.begin(); it!=objects.end(); it++) {
      for (map<SceneObject*, LuaScriptInstance*>::iterator mapIt=mLuaScriptMap.begin(); mapIt!=mLuaScriptMap.end(); mapIt++) {
        if (mapIt->first == (*it)) {

          if (DEBUG) {
            cout << "AssetManager: Removing From Lua Script Map " << (*it)->getUuid() << endl;
          }
          mLuaScriptMap.erase(mapIt);
          break;
        }
      }
    }
  }

  void AssetManager::cleanupScene(Scene* scene) {
    // TODO
    // Remove AssetDefinitions
    // Remove LuaScriptMap Entries for scene objects in scene.
  }

} // End of Dream

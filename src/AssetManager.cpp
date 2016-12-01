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
        for (vector<AssetInstance*>::iterator instanceIt = mAssetInstances.begin();
             instanceIt != mAssetInstances.end(); instanceIt++) {
            delete (*instanceIt);
        }
    }

    bool AssetManager::createAllAssetInstances(Scene* activeScene) {

        if (!activeScene){
            cerr << "AssetManager: Cannot create assets, no active scene." << endl;
            return false;
        }

        // Other Assets
        vector<SceneObject*> scenegraph = activeScene->getScenegraphVector();
        vector<SceneObject*>::iterator scenegraphIterator;

        vector<string> assetInstanceUUIDsToLoad;
        vector<string>::iterator assetInstanceUUIDIterator;

        for (scenegraphIterator = scenegraph.begin(); scenegraphIterator != scenegraph.end(); scenegraphIterator++) {
            SceneObject* currentSceneObject = (*scenegraphIterator);
            assetInstanceUUIDsToLoad = currentSceneObject->getAssetInstanceUUIDsToLoad();

            for (assetInstanceUUIDIterator = assetInstanceUUIDsToLoad.begin();
                 assetInstanceUUIDIterator != assetInstanceUUIDsToLoad.end(); assetInstanceUUIDIterator++) {

                string assetDefinitionUUID = *assetInstanceUUIDIterator;
                AssetInstance* newAsset = createAssetInstanceFromDefinitionUUID(currentSceneObject, assetDefinitionUUID);
                if (newAsset == nullptr) {
                    AssetDefinition* definition = getAssetDefinitionByUUID(assetDefinitionUUID);
                    cerr << "AssetManager: Unable to instanciate asset instance for "
                         << definition->getName() << " (" << definition->getUUID() << ")" << endl;
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

    AssetInstance* AssetManager::createAssetInstanceFromDefinitionUUID(SceneObject* sceneObject, string uuid) {
        AssetDefinition* assetDefinition = getAssetDefinitionByUUID(uuid);
        return createAssetInstance(sceneObject, assetDefinition);
    }

    void AssetManager::showStatus() {}

    AssetInstance* AssetManager::createAssetInstance(SceneObject* sceneObject,AssetDefinition* definition) {
        AssetInstance* retval = nullptr;

        cout << "AssetManager: Creating Asset Intance of: ("
             << definition->getType() << ") " << definition->getName()
             << ", for SceneObject: " << sceneObject->getNameUUIDString()
             << endl;

        if(definition->isTypeAnimation()) {
            retval = createAnimationInstance(sceneObject, definition);
        } else if (definition->isTypeAudio()) {
            retval = createAudioInstance(sceneObject, definition);
        } else if (definition->isTypeModel()) {
            retval = createModelInstance(sceneObject, definition);
        } else if (definition->isTypeScript()){
            retval = createScriptInstance(sceneObject, definition);
        } else if (definition->isTypeShader()) {
            retval = createShaderInstance(sceneObject, definition);
        } else if (definition->isTypePhysicsObject()) {
            retval = createPhysicsObjectInstance(sceneObject,definition);
        } else if (definition->isTypeLight()) {
            retval = createLightInstance(sceneObject, definition);
        } else if (definition->isTypeSprite()) {
            retval = createSpriteInstance(sceneObject, definition);
        }

        if (retval != nullptr) {
            cout << "AssetManager: Loading Asset Data for " << definition->getName() << endl;
            retval->load(mProjectPath);
        }

        return retval;
    }

    void AssetManager::setProjectPath(string projectPath) {
        mProjectPath = projectPath;
    }

    AssetInstance* AssetManager::createPhysicsObjectInstance(SceneObject *sceneObject, AssetDefinition* definition) {
        cout << "AssetManager: Creating Physics Object Asset Instance." << endl;

        PhysicsObjectInstance* retval = new PhysicsObjectInstance(definition,sceneObject->getTransform());

        if (sceneObject && retval) {
            sceneObject->setPhysicsObjectInstance(retval);
            mSceneObjectsWithPhysicsObjects.push_back(sceneObject);
        }
        return retval;
    }

    AssetInstance* AssetManager::createAnimationInstance(SceneObject* sceneObject, AssetDefinition* definition) {
        cout << "AssetManager: Creating animation asset instance." << endl;
        AnimationInstance* retval = new AnimationInstance(definition,sceneObject->getTransform());

        if (sceneObject && retval) {
            sceneObject->setAnimationInstance(retval);
        }
        return retval;
    }

    AssetInstance* AssetManager::createAudioInstance(SceneObject* sceneObject, AssetDefinition* definition) {
        cout << "AssetManager: Creating audio asset instance." << endl;
        AudioInstance* retval = new AudioInstance(
                    definition,
                    sceneObject->getTransform()
                    );

        if (sceneObject && retval) {
            sceneObject->setAudioInstance(retval);
        }
        return retval;
    }

    AssetInstance* AssetManager::createModelInstance(SceneObject* sceneObject, AssetDefinition* definition) {
        cout << "AssetManager: Creating model asset instance." << endl;
        AssimpModelInstance* retval = nullptr;

        if (definition->isModelFormatAssimp()) {
            retval = new AssimpModelInstance(
                        definition,
                        sceneObject->getTransform()
                        );
        }

        if (sceneObject && retval) {
            sceneObject->setModelInstance(retval);
        }

        return retval;
    }

    AssetInstance* AssetManager::createScriptInstance(SceneObject* sceneObject, AssetDefinition* definition) {
        cout << "AssetManager: Creating script asset instance." << endl;
        LuaScriptInstance* retval = new LuaScriptInstance(
            definition,
            sceneObject->getTransform()
        );

        if (sceneObject && retval) {
            sceneObject->setScriptInstance(retval);
            insertIntoLuaScriptMap(sceneObject,retval);
        }

        return retval;
    }

    AssetInstance* AssetManager::createShaderInstance(SceneObject* sceneObject, AssetDefinition* definition) {
        cout << "AssetManager: Creating Shader asset instance." << endl;
        ShaderInstance* retval = nullptr;
        retval = new ShaderInstance(definition,sceneObject->getTransform());
        if (sceneObject && retval) {
            sceneObject->setShaderInstance(retval);
        }
        return retval;
    }

    vector<SceneObject*> AssetManager::getSceneObjectsWithPhysicsObjects() {
        return mSceneObjectsWithPhysicsObjects;
    }

    AssetInstance* AssetManager::createLightInstance(SceneObject *sceneObject, AssetDefinition* definition) {
        cout << "AssetManager: Creating Light Asset instance." << endl;
        LightInstance* retval = new LightInstance(
            definition,
            sceneObject->getTransform()
        );

        if (sceneObject && retval) {
            sceneObject->setLightInstance(retval);
        }
        return retval;
    }

    AssetInstance* AssetManager::createSpriteInstance(SceneObject *sceneObject, AssetDefinition* definition) {
        cout << "AssetManager: Creating Sprite Asset instance." << endl;
        SpriteInstance* retval = new SpriteInstance(
          definition,
          sceneObject->getTransform()
        );


        if (sceneObject && retval) {
            sceneObject->setSpriteInstance(retval);
        }

        return retval;
    }

    AssetInstance* AssetManager::getAssetInstanceByUUID(string uuid) {
        vector<AssetInstance*>::iterator assetIt;
        AssetInstance* retval = nullptr;
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
        cout << "AssetManager: Remove Asset is not yet Implemented" << endl;
    }

    size_t AssetManager::getNumberOfAssetDefinitions() {
        return mAssetDefinitions.size();
    }

    void AssetManager::loadAssetDefinitionsFromJson(nlohmann::json jsonAssetArray) {
        cout << "AssetManager: Loading Assets from JSON Array" << endl;
        for (nlohmann::json::iterator it = jsonAssetArray.begin(); it != jsonAssetArray.end(); ++it) {
            addAssetDefinition(new AssetDefinition((*it)));
        }
    }

    AssetDefinition* AssetManager::getAssetDefinitionByUUID(string uuid) {
        AssetDefinition* retval = nullptr;
        for (vector<AssetDefinition*>::iterator it = mAssetDefinitions.begin(); it != mAssetDefinitions.end(); it++) {
            if ((*it)->getUUID().compare(uuid) == 0) {
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
} // End of Dream

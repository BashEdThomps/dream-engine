/*
 * Scene
 *
 * Created: 13 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#include "Scene.h"
#include "Components/Audio/IAudioComponent.h"

namespace Dream
{
    Scene::Scene
    (
            nlohmann::json jsonScene,
            string projectPath,
            vector<AssetDefinition*>* assetDefs,
            IAudioComponent* audioComp
    )
    {
        mJson = jsonScene;
        mAudioComponent = audioComp;
        setProjectPath(projectPath);
        mAssetDefinitions = assetDefs;
        mGravity = {0.0f,0.0f,0.0f};
        mClearColour = {0.0f,0.0f,0.0f,0.0f};
        mAmbientLightColour = {0.0f,0.0f,0.0f,0.0f};
        mRootSceneObject = nullptr;
        mDefaultCameraTransform = new Transform3D();
        if (!jsonScene[SCENE_JSON_UUID].is_null())
        {
            mUuid = jsonScene[SCENE_JSON_UUID];
        }
        if (!jsonScene[SCENE_JSON_NAME].is_null())
        {
            mName = jsonScene[SCENE_JSON_NAME];
        }
        if (!jsonScene[SCENE_JSON_NOTES].is_null())
        {
            mNotes = jsonScene[SCENE_JSON_NOTES];
        }
        loadPhysicsMetadata(jsonScene);
        loadGraphicsMetadata(jsonScene);

        nlohmann::json sceneObjects = jsonScene[SCENE_JSON_SCENE_OBJECTS];
        if (!sceneObjects.is_null() && sceneObjects.is_array())
        {
            loadSceneObjectMetadata(sceneObjects,nullptr);
        }
    }

    void
    Scene::loadGraphicsMetadata
    (nlohmann::json jsonScene)
    {
        loadDefaultCameraTransform(jsonScene[SCENE_JSON_CAMERA]);
        loadClearColour(jsonScene[SCENE_JSON_CLEAR_COLOUR]);
        loadAmbientLightColour(jsonScene[SCENE_JSON_AMBIENT_LIGHT_COLOUR]);

    }

    void
    Scene::loadPhysicsMetadata
    (nlohmann::json jsonScene)
    {
        nlohmann::json gravity = jsonScene[SCENE_JSON_GRAVITY];
        nlohmann::json physDebug = jsonScene[SCENE_JSON_PHYSICS_DEBUG];

        if (!gravity.is_null())
        {
            mGravity[0] = gravity[SCENE_JSON_X];
            mGravity[1] = gravity[SCENE_JSON_Y];
            mGravity[2] = gravity[SCENE_JSON_Z];
        }
        else
        {
            mGravity[0] = 0.0f;
            mGravity[1] = 0.0f;
            mGravity[2] = 0.0f;
        }

        if (!physDebug.is_null() && physDebug.is_boolean())
        {
            mPhysicsDebug = physDebug;
        }
        else
        {
            mPhysicsDebug = false;
        }
    }

    vector<float>
    Scene::getGravity
    ()
    {
        return mGravity;
    }

    void
    Scene::loadDefaultCameraTransform
    (nlohmann::json camera)
    {
        if (!camera.is_null())
        {
            nlohmann::json translation = camera[SCENE_JSON_TRANSLATION];
            mDefaultCameraTransform->setTranslation(
                        translation[SCENE_JSON_X],
                        translation[SCENE_JSON_Y],
                        translation[SCENE_JSON_Z]
                        );

            nlohmann::json rotation = camera[SCENE_JSON_ROTATION];
            mDefaultCameraTransform->setRotation(
                        rotation[SCENE_JSON_X],
                        rotation[SCENE_JSON_Y],
                        rotation[SCENE_JSON_Z]
                        );

            if (!camera[SCENE_JSON_MOVEMENT_SPEED].is_null())
            {
                setCameraMovementSpeed(camera[SCENE_JSON_MOVEMENT_SPEED]);
            }

        }
        else
        {
            mDefaultCameraTransform->setTranslation(0.0f, 0.0f, 0.0f);
            mDefaultCameraTransform->setRotation(0.0f, 0.0f, 0.0f);
        }
    }

    string
    Scene::getNameAndUuidString
    ()
    {
        return getName() + " (" + getUuid() + ")";
    }

    Scene::~Scene
    ()
    {
        if (DEBUG)
        {
            cout << "Scene: Destroying Object" << endl;
        }

        if (mDefaultCameraTransform != nullptr)
        {
            delete mDefaultCameraTransform;
        }
    }

    bool
    Scene::isScenegraphVectorEmpty
    ()
    {
        return mScenegraphVector.empty();
    }

    string
    Scene::getName
    ()
    {
        return mName;
    }

    string
    Scene::getUuid
    ()
    {
        return mUuid;
    }

    void
    Scene::setUuid
    (string uuid)
    {
        mUuid = uuid;
    }

    void
    Scene::setName
    (string name)
    {
        mName = name;
    }

    void
    Scene::loadSceneObjectMetadata
    (nlohmann::json jsonArray, SceneObject* parent)
    {
        if (!jsonArray.is_null())
        {
            for (nlohmann::json::iterator it = jsonArray.begin(); it != jsonArray.end(); ++it)
            {
                SceneObject *nextSceneObject = new SceneObject(*it);
                if (parent != nullptr)
                {
                    parent->addChild(nextSceneObject);
                }
                else
                {
                    setRootSceneObject(nextSceneObject);
                }
                if (!((*it)[SCENE_OBJECT_CHILDREN]).is_null())
                {
                    loadSceneObjectMetadata((*it)[SCENE_OBJECT_CHILDREN],nextSceneObject);
                }
                if (DEBUG)
                {
                    nextSceneObject->showStatus();
                }
            }
        }
    }

    bool
    Scene::hasSceneObect
    (SceneObject *obj)
    {
        return mRootSceneObject == obj || mRootSceneObject->isParentOfDeep(obj);
    }

    SceneObject*
    Scene::getSceneObjectByUuid
    (string uuid)
    {
        for (vector<SceneObject*>::iterator it = mScenegraphVector.begin(); it != mScenegraphVector.end(); it++)
        {
            if ((*it)->hasUuid(uuid))
            {
                return (*it);
            }
        }
        return nullptr;
    }

    SceneObject*
    Scene::getSceneObjectByName
    (string name)
    {
        for (vector<SceneObject*>::iterator it = mScenegraphVector.begin(); it != mScenegraphVector.end(); it++)
        {
            if ((*it)->hasName(name))
            {
                return (*it);
            }
        }
        return nullptr;
    }

    size_t
    Scene::getNumberOfSceneObjects
    ()
    {
        return mScenegraphVector.size();
    }

    void
    Scene::showStatus
    ()
    {
        cout << "Scene" << endl;
        cout << "{" << endl;
        cout << "\tUUID: " << mUuid << endl;
        cout << "\tName: " << mName << endl;
        cout << "\tCamera Transform: " << endl;
        cout << "\tTranslation: " << String::vec3ToString(mDefaultCameraTransform->getTranslation()) << endl;
        cout << "\tRotation: " << String::vec3ToString(mDefaultCameraTransform->getRotation())    << endl;
        cout << "\tScene Objects: " << getNumberOfSceneObjects() << endl;
        cout << "}" << endl;
        showScenegraph();
    }

    void
    Scene::showScenegraph
    ()
    {
        if (mRootSceneObject == nullptr)
        {
            cout << "Scene: Scenegraph is empty (no root SceneObject)" << endl;
            return;
        }
        if (mScenegraphVector.empty())
        {
            generateScenegraphVector();
        }
        for(vector<SceneObject*>::iterator it = mScenegraphVector.begin(); it != mScenegraphVector.end(); it++)
        {
            cout << (*it)->getNameUuidString() << endl;
        }
    }

    void
    Scene::setRootSceneObject
    (SceneObject* root)
    {
        mRootSceneObject = root;
    }

    SceneObject*
    Scene::getRootSceneObject
    ()
    {
        return mRootSceneObject;
    }

    void
    Scene::generateScenegraphVector
    ()
    {
        mScenegraphVector.clear();
        if (mRootSceneObject != nullptr)
        {
            mRootSceneObject->getChildrenVectorDeep(&mScenegraphVector);
        }
    }

    vector<SceneObject*>
    Scene::getScenegraphVector
    ()
    {
        return mScenegraphVector;
    }

    glm::vec3
    Scene::getDefaultCameraTranslation
    ()
    {
        return mDefaultCameraTransform->getTranslation();
    }

    glm::vec3
    Scene::getDefaultCameraRotation
    ()
    {
        return mDefaultCameraTransform->getRotation();
    }

    void
    Scene::setCameraMovementSpeed
    (float speed)
    {
        mCameraMovementSpeed = speed;
    }

    float
    Scene::getCameraMovementSpeed
    ()
    {
        return mCameraMovementSpeed;
    }

    void
    Scene::loadClearColour
    (nlohmann::json jsonClearColour)
    {
        if (!jsonClearColour.is_null())
        {
            mClearColour[0] = jsonClearColour[SCENE_JSON_RED];
            mClearColour[1] = jsonClearColour[SCENE_JSON_GREEN];
            mClearColour[2] = jsonClearColour[SCENE_JSON_BLUE];
            mClearColour[3] = jsonClearColour[SCENE_JSON_ALPHA];
        }
    }

    void
    Scene::loadAmbientLightColour
    (nlohmann::json jsonAmbientLight)
    {
        if (!jsonAmbientLight.is_null())
        {
            mAmbientLightColour[0] = jsonAmbientLight[SCENE_JSON_RED];
            mAmbientLightColour[1] = jsonAmbientLight[SCENE_JSON_GREEN];
            mAmbientLightColour[2] = jsonAmbientLight[SCENE_JSON_BLUE];
            mAmbientLightColour[3] = jsonAmbientLight[SCENE_JSON_ALPHA];
        }
    }

    vector<float>
    Scene::getAmbientLightColour
    ()
    {
        return mAmbientLightColour;
    }

    vector<float>
    Scene::getClearColour
    ()
    {
        return mClearColour;
    }

    void
    Scene::addToDeleteQueue
    (SceneObject* object)
    {
        mDeleteQueue.push_back(object);
    }

    void
    Scene::clearDeleteQueue
    ()
    {
        mDeleteQueue.clear();
    }

    void
    Scene::destroyDeleteQueue
    ()
    {
        if (!mDeleteQueue.empty())
        {
            vector<SceneObject*>::iterator it;
            for(it=mDeleteQueue.begin(); it!=mDeleteQueue.end(); it++)
            {
                SceneObject* obj = (*it);
                SceneObject* parent = obj->getParent();
                if (parent != nullptr)
                {
                    parent->removeChild(obj);
                }
                delete obj;
            }
        }
    }

    void
    Scene::findDeletedSceneObjects
    ()
    {
        if (VERBOSE)
        {
            cout << "Scene: Cleanup Deleted SceneObjects Called" << endl;
        }
        vector<SceneObject*>::iterator it;
        for(it=mScenegraphVector.begin(); it!=mScenegraphVector.end(); it++)
        {
            if ((*it)->getDeleteFlag())
            {
                addToDeleteQueue(*it);
            }
        }
    }

    vector<SceneObject*>
    Scene::getDeleteQueue
    ()
    {
        return mDeleteQueue;
    }

    bool
    Scene::createAllAssetInstances
    ()
    {
        if (VERBOSE)
        {
            cout << "Secne: Create All Asset Instances Called" << endl;
        }
        generateScenegraphVector();
        vector<SceneObject*> scenegraph = getScenegraphVector();
        vector<SceneObject*>::iterator sgIt;
        for (sgIt = scenegraph.begin(); sgIt != scenegraph.end(); sgIt++)
        {
            SceneObject* sceneObj = (*sgIt);
            // Not loaded && not marked to delete
            if (!sceneObj->getLoadedFlag() && !sceneObj->getDeleteFlag())
            {
                if (!createAssetInstancesForSceneObject(sceneObj))
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool
    Scene::createAssetInstancesForSceneObject
    (SceneObject* currentSO)
    {
        vector<string> aiUuidsToLoad;
        vector<string>::iterator aiUuidIt;
        aiUuidsToLoad = currentSO->getAssetDefUuidsToLoad();
        for (aiUuidIt = aiUuidsToLoad.begin(); aiUuidIt != aiUuidsToLoad.end(); aiUuidIt++)
        {
            string aDefUuid = *aiUuidIt;
            IAssetInstance* newAsset = createAssetInstanceFromDefinitionUuid(currentSO, aDefUuid);
            if (newAsset == nullptr)
            {
                AssetDefinition* definition = getAssetDefinitionByUuid(aDefUuid);
                cerr << "Scene: Unable to instanciate asset instance for "
                     << definition->getName() << " (" << definition->getUuid() << ")" << endl;
                return false;
            }
        }
        currentSO->setLoadedFlag(true);
        return currentSO->getLoadedFlag();
    }

    IAssetInstance*
    Scene::createAssetInstanceFromDefinitionUuid
    (SceneObject* sceneObject, string uuid)
    {
        AssetDefinition* assetDefinition = getAssetDefinitionByUuid(uuid);
        return createAssetInstance(sceneObject, assetDefinition);
    }

    IAssetInstance*
    Scene::createAssetInstance
    (SceneObject* sceneObject,AssetDefinition* definition)
    {
        IAssetInstance* retval = nullptr;
        if (DEBUG)
        {
            cout << "Scene: Creating Asset Intance of: ("
                 << definition->getType() << ") " << definition->getName()
                 << ", for SceneObject: " << sceneObject->getNameUuidString()
                 << endl;
        }

        if(definition->isTypeAnimation())
        {
            retval = createAnimationInstance(sceneObject, definition);
        }
        else if (definition->isTypeAudio())
        {
            retval = createAudioInstance(sceneObject, definition);
        }
        else if (definition->isTypeModel())
        {
            retval = createModelInstance(sceneObject, definition);
        }
        else if (definition->isTypeScript())
        {
            retval = createScriptInstance(sceneObject, definition);
        }
        else if (definition->isTypeShader())
        {
            retval = createShaderInstance(sceneObject, definition);
        }
        else if (definition->isTypePhysicsObject())
        {
            retval = createPhysicsObjectInstance(sceneObject,definition);
        }
        else if (definition->isTypeLight())
        {
            retval = createLightInstance(sceneObject, definition);
        }
        else if (definition->isTypeSprite())
        {
            retval = createSpriteInstance(sceneObject, definition);
        }
        else if (definition->isTypeFont())
        {
            retval = createFontInstance(sceneObject,definition);
        }

        if (retval != nullptr)
        {
            if (DEBUG)
            {
                cout << "Scene: Loading Asset Data for " << definition->getName() << endl;
            }
            if (!retval->load(mProjectPath))
            {
                cerr << "Scene: Failed to create instance of " << definition->getName() << endl;
                return nullptr;
            }
        }
        return retval;
    }

    void
    Scene::setProjectPath
    (string projectPath)
    {
        mProjectPath = projectPath;
    }

    PhysicsObjectInstance*
    Scene::createPhysicsObjectInstance
    (SceneObject *sceneObject, AssetDefinition* definition)
    {
        if (DEBUG)
        {
            cout << "Scene: Creating Physics Object Asset Instance." << endl;
        }
        PhysicsObjectInstance* retval = new PhysicsObjectInstance(definition,sceneObject->getTransform(),mAssetDefinitions);
        sceneObject->setPhysicsObjectInstance(retval);
        return retval;
    }

    AnimationInstance*
    Scene::createAnimationInstance
    (SceneObject* sceneObject, AssetDefinition* definition)
    {
        if (DEBUG)
        {
            cout << "Scene: Creating Animation asset instance." << endl;
        }
        AnimationInstance* retval = new AnimationInstance(definition,sceneObject->getTransform());
        sceneObject->setAnimationInstance(retval);
        return retval;
    }

    IAudioInstance*
    Scene::createAudioInstance
    (SceneObject* sceneObject, AssetDefinition* definition)
    {
        if (DEBUG)
        {
            cout << "Scene: Creating Audio asset instance." << endl;
        }
        IAudioInstance* retval = mAudioComponent->newAudioInstance(definition,sceneObject->getTransform());
        sceneObject->setAudioInstance(retval);
        return retval;
    }

    AssimpModelInstance* Scene::createModelInstance(SceneObject* sceneObject, AssetDefinition* definition)
    {
        if (DEBUG)
        {
            cout << "Scene: Creating Model asset instance." << endl;
        }
        AssimpModelInstance* retval = nullptr;
        retval = new AssimpModelInstance(definition,sceneObject->getTransform());
        sceneObject->setModelInstance(retval);
        return retval;
    }

    LuaScriptInstance*
    Scene::createScriptInstance
    (SceneObject* sceneObject, AssetDefinition* definition)
    {
        if (DEBUG)
        {
            cout << "Scene: Creating Script asset instance." << endl;
        }
        LuaScriptInstance* retval = new LuaScriptInstance(definition, sceneObject->getTransform());
        sceneObject->setScriptInstance(retval);
        insertIntoLuaScriptMap(sceneObject,retval);
        return retval;
    }

    ShaderInstance*
    Scene::createShaderInstance
    (SceneObject* sceneObject, AssetDefinition* definition)
    {
        if (DEBUG)
        {
            cout << "Scene: Creating Shader asset instance." << endl;
        }
        ShaderInstance* retval = new ShaderInstance(definition,sceneObject->getTransform());
        sceneObject->setShaderInstance(retval);
        return retval;
    }

    LightInstance*
    Scene::createLightInstance
    (SceneObject *sceneObject, AssetDefinition* definition)
    {
        if (DEBUG)
        {
            cout << "Scene: Creating Light Asset instance." << endl;
        }
        LightInstance* retval = new LightInstance(definition,sceneObject->getTransform());
        sceneObject->setLightInstance(retval);
        return retval;
    }

    SpriteInstance*
    Scene::createSpriteInstance
    (SceneObject *sceneObject, AssetDefinition* definition)
    {
        if (DEBUG)
        {
            cout << "Scene: Creating Sprite Asset instance." << endl;
        }
        SpriteInstance* retval = new SpriteInstance(definition,sceneObject->getTransform());
        sceneObject->setSpriteInstance(retval);
        return retval;
    }

    FontInstance*
    Scene::createFontInstance
    (SceneObject *sceneObject, AssetDefinition* definition)
    {
        if (DEBUG)
        {
            cout << "Scene: Creating Font Asset instance." << endl;
        }
        FontInstance* retval = new FontInstance(definition,sceneObject->getTransform());
        sceneObject->setFontInstance(retval);
        return retval;
    }

    map<SceneObject*,LuaScriptInstance*>*
    Scene::getLuaScriptMap
    ()
    {
        return &mLuaScriptMap;
    }

    void
    Scene::insertIntoLuaScriptMap
    (SceneObject* sceneObject,LuaScriptInstance* script)
    {
        mLuaScriptMap.insert(pair<SceneObject*,LuaScriptInstance*>(sceneObject,script));
    }


    void
    Scene::findDeletedScripts()
    {
        if (VERBOSE)
        {
            cout << "Scene: Cleanup Deleted Scripts Called" << endl;
        }
        vector<SceneObject*> objects = getDeleteQueue();
        for (vector<SceneObject*>::iterator it=objects.begin(); it!=objects.end(); it++)
        {
            removeFromLuaScriptMap(*it);
        }
    }

    void
    Scene::removeFromLuaScriptMap
    (SceneObject* it)
    {
        map<SceneObject*, LuaScriptInstance*>::iterator mapIt;
        for (mapIt=mLuaScriptMap.begin(); mapIt!=mLuaScriptMap.end(); mapIt++)
        {
            if (mapIt->first == it)
            {
                if (DEBUG)
                {
                    cout << "Scene: Removing From Lua Script Map " << it->getUuid() << endl;
                }
                mLuaScriptMap.erase(mapIt);
                break;
            }
        }
    }

    AssetDefinition*
    Scene::getAssetDefinitionByUuid
    (string uuid)
    {
        AssetDefinition* retval = nullptr;
        vector<AssetDefinition*>::iterator it;
        for (it = mAssetDefinitions->begin(); it != mAssetDefinitions->end(); it++) {
            if ((*it)->getUuid().compare(uuid) == 0) {
                retval = (*it);
                break;
            }
        }
        return retval;
    }

    bool
    Scene::getPhysicsDebug
    ()
    {
        return mPhysicsDebug;
    }

    nlohmann::json
    Scene::toJson
    ()
    {
        return mJson;
    }

    string
    Scene::getNotes
    ()
    {
        return mNotes;
    }

    void
    Scene::setNotes
    (string notes)
    {
        mNotes = notes;
    }

} // End of Dream

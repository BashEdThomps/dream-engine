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
#include "Components/Audio/AudioComponent.h"
#include <functional>
#include "Lua/LuaEngine.h"

namespace Dream
{
    Scene::Scene
    ()
         : mActive(false)
    {

    }

    Scene::Scene
    (
            nlohmann::json jsonScene,
            string projectPath,
            vector<AssetDefinition> assetDefs,
            shared_ptr<AudioComponent> audioComp,
            shared_ptr<LuaEngine> luaEngine
    )
        :
          mActive(false),
          mAmbientLightColour({0.0f,0.0f,0.0f,0.0f}),
          mClearColour({0.0f,0.0f,0.0f,0.0f}),
          mGravity ({0.0f,0.0f,0.0f}),
          mAssetDefinitions(assetDefs),
          mAudioComponent(audioComp),
          mJson(jsonScene),
          mLuaEngine(luaEngine),
          mProjectPath(projectPath)
    {


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
            mDefaultCameraTransform.setTranslation(
                        translation[SCENE_JSON_X],
                        translation[SCENE_JSON_Y],
                        translation[SCENE_JSON_Z]
                        );

            nlohmann::json rotation = camera[SCENE_JSON_ROTATION];
            mDefaultCameraTransform.setRotation(
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
            mDefaultCameraTransform.setTranslation(0.0f, 0.0f, 0.0f);
            mDefaultCameraTransform.setRotation(0.0f, 0.0f, 0.0f);
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
            for (nlohmann::json it : jsonArray)
            {
                SceneObject nextSceneObject(it);
                if (parent != nullptr)
                {
                    parent->addChild(nextSceneObject);
                }
                else
                {
                    setRootSceneObject(nextSceneObject);
                }
                if (!it[SCENE_OBJECT_CHILDREN].is_null())
                {
                    loadSceneObjectMetadata(it[SCENE_OBJECT_CHILDREN],&nextSceneObject);
                }
                if (DEBUG)
                {
                    nextSceneObject.showStatus();
                }
            }
        }
    }

    SceneObject
    Scene::getSceneObjectByUuid
    (string uuid)
    {
        SceneObject result = mRootSceneObject.applyToAll
        (
            function<SceneObject(SceneObject&)>
            (
                [&](SceneObject& so)
                {
                    SceneObject retval;
                    if (so.hasUuid(uuid))
                    {
                        retval = so;
                    }
                    return retval;
                }
            )
        );
        return result;
    }

    SceneObject
    Scene::getSceneObjectByName
    (string name)
    {
        SceneObject result = mRootSceneObject.applyToAll
        (
            function<SceneObject(SceneObject&)>
            (
                [&](SceneObject& so)
                {
                    SceneObject retval;
                    if (so.hasName(name))
                    {
                        retval = so;
                    }

                    return retval;
                }
            )
        );
        return result;
    }

    size_t
    Scene::getNumberOfSceneObjects
    ()
    {
        size_t count = 0;
        mRootSceneObject.applyToAll
        (
            function<void(SceneObject&)>
            (
                [&](SceneObject& obj)
                {
                    count++;
                }
            )
        );
        return count;
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
        cout << "\tTranslation: " << String::vec3ToString(mDefaultCameraTransform.getTranslation()) << endl;
        cout << "\tRotation: " << String::vec3ToString(mDefaultCameraTransform.getRotation())    << endl;
        cout << "\tScene Objects: " << getNumberOfSceneObjects() << endl;
        cout << "}" << endl;
        showScenegraph();
    }

    void
    Scene::showScenegraph
    ()
    {
        if (DEBUG)
        {
            cout << "Scene: Showing Scenegraph..." << endl;
            mRootSceneObject.applyToAll
            (
                function<void(SceneObject&)>
                (
                    [&](SceneObject& obj)
                    {
                        cout << obj.getNameAndUuidString() << endl;
                    }
                )
            );
        }
    }

    void
    Scene::setRootSceneObject
    (SceneObject& root)
    {
        mRootSceneObject = root;
    }

    SceneObject
    Scene::getRootSceneObject
    ()
    {
        return mRootSceneObject;
    }

    glm::vec3
    Scene::getDefaultCameraTranslation
    ()
    {
        return mDefaultCameraTransform.getTranslation();
    }

    glm::vec3
    Scene::getDefaultCameraRotation
    ()
    {
        return mDefaultCameraTransform.getRotation();
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
    (SceneObject object)
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
    Scene::findDeletedSceneObjects
    ()
    {
        if (VERBOSE)
        {
            cout << "Scene: Cleanup Deleted SceneObjects Called" << endl;
        }

        mRootSceneObject.applyToAll
        (
            function<void(SceneObject&)>
            (
                [&](SceneObject& it)
                {
                    if (it.getDeleteFlag())
                    {
                        addToDeleteQueue(it);
                    }
                }
            )
        );
    }

    vector<SceneObject>
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

        bool result = mRootSceneObject.applyToAll
        (
            function<bool(SceneObject&)>
            (
                [&](SceneObject& sceneObj)
                {
                    // Not loaded && not marked to delete
                    if (!sceneObj.getLoadedFlag() && !sceneObj.getDeleteFlag())
                    {
                        if (!createAssetInstancesForSceneObject(sceneObj))
                        {
                            return false;
                        }
                    }
                    return true;
                }
            )
        );

        return result;
    }

    bool
    Scene::createAssetInstancesForSceneObject
    (SceneObject& currentSO)
    {
        vector<string> aiUuidsToLoad;
        vector<string>::iterator aiUuidIt;
        aiUuidsToLoad = currentSO.getAssetDefUuidsToLoad();
        for (aiUuidIt = aiUuidsToLoad.begin(); aiUuidIt != aiUuidsToLoad.end(); aiUuidIt++)
        {
            string aDefUuid = *aiUuidIt;
            shared_ptr<IAssetInstance> newAsset = createAssetInstanceFromDefinitionUuid(currentSO, aDefUuid);
            if (newAsset == nullptr)
            {
                AssetDefinition definition = getAssetDefinitionByUuid(aDefUuid);
                cerr << "Scene: Unable to instanciate asset instance for "
                     << definition.getName() << " (" << definition.getUuid() << ")" << endl;
                return false;
            }
        }
        currentSO.setLoadedFlag(true);
        return currentSO.getLoadedFlag();
    }

    shared_ptr<IAssetInstance>
    Scene::createAssetInstanceFromDefinitionUuid
    (SceneObject& sceneObject, string uuid)
    {
        AssetDefinition assetDefinition = getAssetDefinitionByUuid(uuid);
        return createAssetInstance(sceneObject, assetDefinition);
    }

    shared_ptr<IAssetInstance>
    Scene::createAssetInstance
    (SceneObject& sceneObject,AssetDefinition& definition)
    {
        shared_ptr<IAssetInstance> retval;
        if (DEBUG)
        {
            cout << "Scene: Creating Asset Intance of: ("
                 << definition.getType() << ") " << definition.getName()
                 << ", for SceneObject: " << sceneObject.getNameAndUuidString()
                 << endl;
        }

        if(definition.isTypeAnimation())
        {
            retval = createAnimationInstance(sceneObject, definition);
        }
        else if (definition.isTypeAudio())
        {
            retval = createAudioInstance(sceneObject, definition);
        }
        else if (definition.isTypeModel())
        {
            retval = createModelInstance(sceneObject, definition);
        }
        else if (definition.isTypeScript())
        {
            retval = createScriptInstance(sceneObject, definition);
        }
        else if (definition.isTypeShader())
        {
            retval = createShaderInstance(sceneObject, definition);
        }
        else if (definition.isTypePhysicsObject())
        {
            retval = createPhysicsObjectInstance(sceneObject,definition);
        }
        else if (definition.isTypeLight())
        {
            retval = createLightInstance(sceneObject, definition);
        }
        else if (definition.isTypeSprite())
        {
            retval = createSpriteInstance(sceneObject, definition);
        }
        else if (definition.isTypeFont())
        {
            retval = createFontInstance(sceneObject,definition);
        }

        if (retval != nullptr)
        {
            if (DEBUG)
            {
                cout << "Scene: Loading Asset Data for " << definition.getName() << endl;
            }
            if (!retval->load(mProjectPath))
            {
                cerr << "Scene: Failed to create instance of " << definition.getName() << endl;
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

    shared_ptr<PhysicsObjectInstance>
    Scene::createPhysicsObjectInstance
    (SceneObject& sceneObject, AssetDefinition& definition)
    {
        if (DEBUG)
        {
            cout << "Scene: Creating Physics Object Asset Instance." << endl;
        }
        shared_ptr<PhysicsObjectInstance> retval;
        Transform3D initial = sceneObject.getInitialTransform();
        retval.make_shared(definition,initial,mAssetDefinitions);
        sceneObject.setPhysicsObjectInstance(retval);
        return retval;
    }

    shared_ptr<AnimationInstance>
    Scene::createAnimationInstance
    (SceneObject& sceneObject, AssetDefinition& definition)
    {
        if (DEBUG)
        {
            cout << "Scene: Creating Animation asset instance." << endl;
        }
        Transform3D t = sceneObject.getInitialTransform();
        shared_ptr<AnimationInstance> retval;
        retval.make_shared(definition,t);
        sceneObject.setAnimationInstance(retval);
        return retval;
    }

    shared_ptr<AudioInstance>
    Scene::createAudioInstance
    (SceneObject& sceneObject, AssetDefinition& definition)
    {
        if (DEBUG)
        {
            cout << "Scene: Creating Audio asset instance." << endl;
        }
        Transform3D t = sceneObject.getInitialTransform();
        shared_ptr<AudioInstance> retval = mAudioComponent->newAudioInstance(definition,t);
        sceneObject.setAudioInstance(retval);
        return retval;
    }

    shared_ptr<AssimpModelInstance>
    Scene::createModelInstance
    (SceneObject& sceneObject, AssetDefinition& definition)
    {
        if (DEBUG)
        {
            cout << "Scene: Creating Model asset instance." << endl;
        }
        Transform3D t = sceneObject.getInitialTransform();
        shared_ptr<AssimpModelInstance> retval;
        retval.make_shared(definition,t);
        sceneObject.setModelInstance(retval);
        return retval;
    }

    shared_ptr<LuaScriptInstance>
    Scene::createScriptInstance
    (SceneObject& sceneObject, AssetDefinition& definition)
    {
        if (DEBUG)
        {
            cout << "Scene: Creating Script asset instance." << endl;
        }
        Transform3D t = sceneObject.getInitialTransform();
        shared_ptr<LuaScriptInstance> retval;
        retval.make_shared(definition, t);
        sceneObject.setScriptInstance(retval);
        mLuaEngine->insertIntoScriptMap(sceneObject,*retval);
        return retval;
    }

    shared_ptr<ShaderInstance>
    Scene::createShaderInstance
    (SceneObject& sceneObject, AssetDefinition& definition)
    {
        if (DEBUG)
        {
            cout << "Scene: Creating Shader asset instance." << endl;
        }
        Transform3D t = sceneObject.getInitialTransform();
        shared_ptr<ShaderInstance> retval;
        retval.make_shared(definition,t);
        sceneObject.setShaderInstance(retval);
        return retval;
    }

    shared_ptr<LightInstance>
    Scene::createLightInstance
    (SceneObject& sceneObject, AssetDefinition& definition)
    {
        if (DEBUG)
        {
            cout << "Scene: Creating Light Asset instance." << endl;
        }
        Transform3D t = sceneObject.getInitialTransform();
        shared_ptr<LightInstance> retval;
        retval.make_shared(definition,t);
        sceneObject.setLightInstance(retval);
        return retval;
    }

    shared_ptr<SpriteInstance>
    Scene::createSpriteInstance
    (SceneObject& sceneObject, AssetDefinition& definition)
    {
        if (DEBUG)
        {
            cout << "Scene: Creating Sprite Asset instance." << endl;
        }
        Transform3D t = sceneObject.getInitialTransform();
        shared_ptr<SpriteInstance> retval;
        retval.make_shared(definition,t);
        sceneObject.setSpriteInstance(retval);
        return retval;
    }

    shared_ptr<FontInstance>
    Scene::createFontInstance
    (SceneObject& sceneObject, AssetDefinition& definition)
    {
        if (DEBUG)
        {
            cout << "Scene: Creating Font Asset instance." << endl;
        }
        Transform3D t = sceneObject.getInitialTransform();
        shared_ptr<FontInstance> retval;
        retval.make_shared(definition,t);
        sceneObject.setFontInstance(retval);
        return retval;
    }

    void
    Scene::findDeletedScripts()
    {
        if (DEBUG)
        {
            cout << "Scene: Cleanup Deleted Scripts Called" << endl;
        }

        vector<SceneObject> objects = getDeleteQueue();
        for (SceneObject it : objects)
        {
            mLuaEngine->removeFromScriptMap(it);
        }
    }

    AssetDefinition
    Scene::getAssetDefinitionByUuid
    (string uuid)
    {
        AssetDefinition retval;
        for (AssetDefinition it : mAssetDefinitions)
        {
            if (it.getUuid().compare(uuid) == 0)
            {
                retval = it;
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

    void Scene::setDeleteFlagOnAllSceneObjects(bool bDelete)
    {
        mRootSceneObject.applyToAll
        (
            function<void(SceneObject&)>
            (
                [&](SceneObject& it)
                {
                    it.setDeleteFlag(bDelete);
                }
            )
        );
    }

    void Scene::setLoadedFlagOnAllSceneObjects(bool load)
    {
        mRootSceneObject.applyToAll
        (
            function<void(SceneObject&)>
            (
                [&](SceneObject& it)
                {
                    it.setLoadedFlag(load);
                }
            )
        );
    }

    bool Scene::isActive()
    {
        return mActive;
    }

    void Scene::setActive(bool active)
    {
        mActive = active;
    }

} // End of Dream

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
#include "ProjectRuntime.h"
#include <functional>
#include <algorithm>
#include "Project.h"

namespace Dream
{
    Scene::Scene
    (Project* project, nlohmann::json jsonScene)
        : mJson(jsonScene),
          mRootSceneObject(nullptr),
          mClearColour({0.0f,0.0f,0.0f,0.0f}),
          mAmbientLightColour({0.0f,0.0f,0.0f,0.0f}),
          mGravity({0.0f,0.0f,0.0f}),
          mState(NOT_LOADED),
          mProject(project)
    {
        loadAllJsonData(mJson);
    }

    Scene::~Scene
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "Scene: Destroying Object" << endl;
        }
    }

    void Scene::loadAllJsonData(nlohmann::json jsonScene)
    {

        if (!jsonScene[Constants::Constants::SCENE_JSON_UUID].is_null())
        {
            mUuid = jsonScene[Constants::Constants::SCENE_JSON_UUID];
        }

        if (!jsonScene[Constants::Constants::SCENE_JSON_NAME].is_null())
        {
            mName = jsonScene[Constants::Constants::SCENE_JSON_NAME];
        }

        if (!jsonScene[Constants::Constants::SCENE_JSON_NOTES].is_null())
        {
            mNotes = jsonScene[Constants::Constants::SCENE_JSON_NOTES];
        }

        loadPhysicsJsonData(jsonScene);
        loadGraphicsJsonData(jsonScene);

        nlohmann::json sceneObjects = jsonScene[Constants::Constants::SCENE_JSON_SCENE_OBJECTS];

        if (!sceneObjects.is_null() && sceneObjects.is_array())
        {
            loadSceneObjectJsonData(sceneObjects,nullptr);
        }
    }

    void
    Scene::loadGraphicsJsonData
    (nlohmann::json jsonScene)
    {
        loadCameraJsonData(jsonScene[Constants::Constants::SCENE_JSON_CAMERA]);
        loadClearColourJsonData(jsonScene[Constants::Constants::SCENE_JSON_CLEAR_COLOUR]);
        loadAmbientLightColourJsonData(jsonScene[Constants::Constants::SCENE_JSON_AMBIENT_LIGHT_COLOUR]);

    }

    void
    Scene::loadPhysicsJsonData
    (nlohmann::json jsonScene)
    {
        nlohmann::json gravity = jsonScene[Constants::Constants::SCENE_JSON_GRAVITY];
        nlohmann::json physDebug = jsonScene[Constants::Constants::SCENE_JSON_PHYSICS_DEBUG];

        if (!gravity.is_null())
        {
            mGravity[0] = gravity[Constants::Constants::SCENE_JSON_X];
            mGravity[1] = gravity[Constants::Constants::SCENE_JSON_Y];
            mGravity[2] = gravity[Constants::Constants::SCENE_JSON_Z];
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

    void
    Scene::loadCameraJsonData
    (nlohmann::json camera)
    {
        if (!camera.is_null())
        {
            nlohmann::json translation = camera[Constants::Constants::SCENE_JSON_TRANSLATION];
            mDefaultCameraTransform.setTranslation(
                        translation[Constants::Constants::SCENE_JSON_X],
                        translation[Constants::Constants::SCENE_JSON_Y],
                        translation[Constants::Constants::SCENE_JSON_Z]
                        );

            nlohmann::json rotation = camera[Constants::Constants::SCENE_JSON_ROTATION];
            mDefaultCameraTransform.setRotation(
                        rotation[Constants::Constants::SCENE_JSON_X],
                        rotation[Constants::Constants::SCENE_JSON_Y],
                        rotation[Constants::Constants::SCENE_JSON_Z]
                        );

            if (!camera[Constants::Constants::SCENE_JSON_MOVEMENT_SPEED].is_null())
            {
                setCameraMovementSpeed(camera[Constants::Constants::SCENE_JSON_MOVEMENT_SPEED]);
            }

        }
        else
        {
            mDefaultCameraTransform.setTranslation(0.0f, 0.0f, 0.0f);
            mDefaultCameraTransform.setRotation(0.0f, 0.0f, 0.0f);
        }
    }

    void
    Scene::loadSceneObjectJsonData
    (nlohmann::json jsonArray, SceneObject* parent)
    {
        if (!jsonArray.is_null())
        {
            for (nlohmann::json it : jsonArray)
            {
                SceneObject *nextSceneObject = new SceneObject(this,it);
                if (parent != nullptr)
                {
                    parent->addChild(nextSceneObject);
                }
                else
                {
                    setRootSceneObject(nextSceneObject);
                }
                if (!it[Constants::SCENE_OBJECT_CHILDREN].is_null())
                {
                    loadSceneObjectJsonData(it[Constants::SCENE_OBJECT_CHILDREN],nextSceneObject);
                }
                if (Constants::DEBUG)
                {
                    nextSceneObject->showStatus();
                }
            }
        }
    }

    void
    Scene::loadClearColourJsonData
    (nlohmann::json jsonClearColour)
    {
        if (!jsonClearColour.is_null())
        {
            mClearColour[0] = jsonClearColour[Constants::Constants::SCENE_JSON_RED];
            mClearColour[1] = jsonClearColour[Constants::Constants::SCENE_JSON_GREEN];
            mClearColour[2] = jsonClearColour[Constants::Constants::SCENE_JSON_BLUE];
            mClearColour[3] = jsonClearColour[Constants::Constants::SCENE_JSON_ALPHA];
        }
    }

    void
    Scene::loadAmbientLightColourJsonData
    (nlohmann::json jsonAmbientLight)
    {
        if (!jsonAmbientLight.is_null())
        {
            mAmbientLightColour[0] = jsonAmbientLight[Constants::Constants::SCENE_JSON_RED];
            mAmbientLightColour[1] = jsonAmbientLight[Constants::Constants::SCENE_JSON_GREEN];
            mAmbientLightColour[2] = jsonAmbientLight[Constants::Constants::SCENE_JSON_BLUE];
            mAmbientLightColour[3] = jsonAmbientLight[Constants::Constants::SCENE_JSON_ALPHA];
        }
    }

    vector<float>
    Scene::getGravity
    ()
    {
        return mGravity;
    }

    string
    Scene::getNameAndUuidString
    ()
    {
        return getName() + " : " + getUuid();
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

    SceneObject*
    Scene::getSceneObjectByUuid
    (string uuid)
    {
        return static_cast<SceneObject*>
        (
            mRootSceneObject->applyToAll
            (
                function<void*(SceneObject*)>
                (
                    [&](SceneObject* currentSo)
                    {
                        if (currentSo->hasUuid(uuid))
                        {
                            return currentSo;
                        }
                        return static_cast<SceneObject*>(0);
                    }
                )
            )
        );
    }

    SceneObject*
    Scene::getSceneObjectByName
    (string name)
    {
        return static_cast<SceneObject*>
        (
            mRootSceneObject->applyToAll
            (
                function<void*(SceneObject*)>
                (
                    [&](SceneObject* currentSo)
                    {
                        if (currentSo->hasName(name))
                        {
                            return currentSo;
                        }
                        return static_cast<SceneObject*>(0);
                    }
                )
            )
        );
    }

    int
    Scene::getNumberOfSceneObjects
    ()
    {
        int count = 0;
        mRootSceneObject->applyToAll
        (
            function<void*(SceneObject*)>
            (
                [&](SceneObject*)
                {
                    count++;
                    return nullptr;
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
        if (mRootSceneObject == nullptr)
        {
            cout << "Scene: Scenegraph is empty (no root SceneObject)" << endl;
            return;
        }

        mRootSceneObject->applyToAll
        (
            function<void*(SceneObject*)>
            (
                [&](SceneObject* obj)
                {
                    obj->showStatus();
                    return nullptr;
                }
            )
        );
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
        if (Constants::DEBUG)
        {
            cout << "Scene: Adding "
                 << object->getNameAndUuidString()
                 << " to delete queue" << endl;
        }
        mDeleteQueue.push_back(object);
    }

    void
    Scene::clearDeleteQueue
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "Scene: clearDeleteQueue" << endl;
        }
        mDeleteQueue.clear();
    }

    void
    Scene::destroyDeleteQueue
    ()
    {
        if (!mDeleteQueue.empty())
        {
            for(SceneObject* obj : mDeleteQueue)
            {
                SceneObject* parent = obj->getParent();
                if (parent != nullptr)
                {
                    parent->removeChild(obj);
                }
                obj->cleanUp();
                delete obj;
            }
        }
        clearDeleteQueue();
    }

    void
    Scene::findDeleteFlaggedSceneObjects
    ()
    {
        if (Constants::VERBOSE)
        {
            cout << "Scene: findDeleteFlaggedSceneObjects Called" << endl;
        }

        mRootSceneObject->applyToAll
        (
            function<void*(SceneObject*)>
            (
                [&](SceneObject* obj)
                {
                    if (obj->getDeleteFlag())
                    {
                        addToDeleteQueue(obj);
                    }
                    return nullptr;
                }
            )
        );
    }

    vector<SceneObject*>
    Scene::getDeleteQueue
    ()
    {
        return mDeleteQueue;
    }

    void
    Scene::createAllAssetInstances
    ()
    {
        if (Constants::VERBOSE)
        {
            cout << "Secne: Create All Asset Instances Called" << endl;
        }

        mRootSceneObject->applyToAll
        (
            function<void*(SceneObject*)>
            (
                [&](SceneObject* sceneObj)
                {
                    // Not loaded && not marked to delete
                    if (!sceneObj->getLoadedFlag() && !sceneObj->getDeleteFlag())
                    {
                        sceneObj->createAssetInstances();
                    }
                    return nullptr;
                }
            )
        );
    }

    void
    Scene::loadAllAssetInstances
    ()
    {
        mRootSceneObject->applyToAll
        (
            function<void*(SceneObject*)>
            (
                [&](SceneObject* sceneObj)
                {
                    // Not loaded && not marked to delete
                    if (!sceneObj->getLoadedFlag())
                    {
                        if(!sceneObj->getDeleteFlag())
                        {
                            sceneObj->loadAssetInstances();
                        }
                    }
                    return nullptr;
                }
            )
        );

        setState(LOADED);
    }
    void
    Scene::findDeleteFlaggedScripts
    ()
    {
        if (Constants::VERBOSE)
        {
            cout << "Scene: Cleanup Deleted Scripts Called" << endl;
        }
        vector<SceneObject*> objects = getDeleteQueue();

        for (SceneObject* it : objects)
        {
            mProject->getRuntime()->getLuaEngine()->removeFromScriptMap(it);
        }
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

    void
    Scene::cleanUpSceneObjects
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "Scene: cleanUpSceneObjects" << endl;
        }
        mRootSceneObject->applyToAll
        (
            function<void*(SceneObject*)>
            (
                [&](SceneObject* obj)
                {
                    obj->setDeleteFlag(true);
                    return nullptr;
                }
            )
        );
    }

    void
    Scene::flush
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "Scene: flush" << endl;
        }
        findDeleteFlaggedSceneObjects();
        findDeleteFlaggedScripts();
        destroyDeleteQueue();
    }

    bool
    Scene::hasUuid
    (string uuid)
    {
        return mUuid.compare(uuid) == 0;
    }

    bool
    Scene::hasName
    (string name)
    {
        return mName.compare(name) == 0;
    }

    void
    Scene::cleanUp
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "Scene: cleanUp called on "
                 <<  getNameAndUuidString()
                 << endl;
        }

        if(getState() == DONE)
        {
           cleanUpSceneObjects();
           flush();
           setState(CLEANED_UP);
        }
        else
        {
            cerr << "Scene: Cannot cleanUp Scene "
                 << getNameAndUuidString()
                 << " State != DONE"
                 << endl;
        }
    }

    SceneState
    Scene::getState
    ()
    {
       return mState;
    }

    void
    Scene::setState
    (SceneState state)
    {
        mState = state;
    }

    Project*
    Scene::getProject
    ()
    {
        return mProject;
    }

    bool
    Scene::isState
    (SceneState state)
    {
        return mState == state;
    }


} // End of Dream

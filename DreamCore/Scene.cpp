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
    (Project* project, nlohmann::json json)
        : mProjectHandle(project),
          mJson(json),
          mRootSceneObjectHandle(nullptr),
          mState(NOT_LOADED)
    {
        nlohmann::json sceneObjects = mJson[Constants::SCENE_JSON_SCENE_OBJECTS];

        if (!sceneObjects.is_null() && sceneObjects.is_array())
        {
            loadSceneObjectJsonData(sceneObjects,nullptr);
        }
    }

    Scene::~Scene
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "Scene: Destroying Object" << endl;
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

    vector<float>
    Scene::getGravity
    ()
    {
        vector<float> gravity;
        gravity.reserve(3);
        gravity[0] = mJson[Constants::SCENE_JSON_GRAVITY][Constants::SCENE_JSON_X];
        gravity[1] = mJson[Constants::SCENE_JSON_GRAVITY][Constants::SCENE_JSON_Y];
        gravity[2] = mJson[Constants::SCENE_JSON_GRAVITY][Constants::SCENE_JSON_Z];
        return gravity;
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
        return mJson[Constants::SCENE_JSON_NAME];
    }

    string
    Scene::getUuid
    ()
    {
       return mJson[Constants::SCENE_JSON_UUID];
    }

    void
    Scene::setUuid
    (string uuid)
    {
        mJson[Constants::SCENE_JSON_UUID] = uuid;
    }

    void
    Scene::setName
    (string name)
    {
        mJson[Constants::SCENE_JSON_NAME] = name ;
    }

    SceneObject*
    Scene::getSceneObjectByUuid
    (string uuid)
    {
        return static_cast<SceneObject*>
        (
            mRootSceneObjectHandle->applyToAll
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
            mRootSceneObjectHandle->applyToAll
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
        mRootSceneObjectHandle->applyToAll
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
        if (Constants::DEBUG)
        {
            cout << "Scene" << endl;
            cout << "{" << endl;
            cout << "\tUUID: " << getUuid() << endl;
            cout << "\tName: " << getName() << endl;
            cout << "\tCamera Transform: " << endl;
            cout << "\tTranslation: " << String::vec3ToString(getDefaultCameraTransform().getTranslation()) << endl;
            cout << "\tRotation: " << String::vec3ToString(getDefaultCameraTransform().getRotation())    << endl;
            cout << "\tScene Objects: " << getNumberOfSceneObjects() << endl;
            cout << "}" << endl;
            showScenegraph();
        }
    }

    void
    Scene::showScenegraph
    ()
    {
        if (mRootSceneObjectHandle == nullptr)
        {
            cout << "Scene: Scenegraph is empty (no root SceneObject)" << endl;
            return;
        }

        mRootSceneObjectHandle->applyToAll
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
        mRootSceneObjectHandle = root;
    }

    SceneObject*
    Scene::getRootSceneObject
    ()
    {
        return mRootSceneObjectHandle;
    }

    void
    Scene::setCameraMovementSpeed
    (float speed)
    {
        mJson[Constants::SCENE_JSON_CAMERA][Constants::SCENE_JSON_MOVEMENT_SPEED] = speed;
    }

    float
    Scene::getCameraMovementSpeed
    ()
    {
        return mJson[Constants::SCENE_JSON_CAMERA][Constants::SCENE_JSON_MOVEMENT_SPEED];
    }

    vector<float>
    Scene::getAmbientLightColour
    ()
    {
        vector<float> ambientColour;
        ambientColour.reserve(4);
        ambientColour[0] = mJson[Constants::SCENE_JSON_AMBIENT_LIGHT_COLOUR][Constants::SCENE_JSON_RED];
        ambientColour[1] = mJson[Constants::SCENE_JSON_AMBIENT_LIGHT_COLOUR][Constants::SCENE_JSON_GREEN];
        ambientColour[2] = mJson[Constants::SCENE_JSON_AMBIENT_LIGHT_COLOUR][Constants::SCENE_JSON_BLUE];
        ambientColour[3] = mJson[Constants::SCENE_JSON_AMBIENT_LIGHT_COLOUR][Constants::SCENE_JSON_ALPHA];
        return ambientColour;
    }

    vector<float>
    Scene::getClearColour
    ()
    {
        vector<float> clearColour;
        clearColour.reserve(4);
        clearColour[0] = mJson[Constants::SCENE_JSON_CLEAR_COLOUR][Constants::SCENE_JSON_RED];
        clearColour[1] = mJson[Constants::SCENE_JSON_CLEAR_COLOUR][Constants::SCENE_JSON_GREEN];
        clearColour[2] = mJson[Constants::SCENE_JSON_CLEAR_COLOUR][Constants::SCENE_JSON_BLUE];
        clearColour[3] = mJson[Constants::SCENE_JSON_CLEAR_COLOUR][Constants::SCENE_JSON_ALPHA];
        return clearColour;
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
                obj->cleanUp();
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

        mRootSceneObjectHandle->applyToAll
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

        mRootSceneObjectHandle->applyToAll
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
        mRootSceneObjectHandle->applyToAll
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
            mProjectHandle->getRuntime()->getLuaEngine()->removeFromScriptMap(it);
        }
    }

    bool
    Scene::getPhysicsDebug
    ()
    {
        return mJson[Constants::SCENE_JSON_PHYSICS_DEBUG];
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
        return mJson[Constants::SCENE_JSON_NOTES];
    }

    void
    Scene::setNotes
    (string notes)
    {
        mJson[Constants::SCENE_JSON_NOTES] = notes;
    }

    void
    Scene::cleanUpSceneObjects
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "Scene: cleanUpSceneObjects" << endl;
        }
        mRootSceneObjectHandle->applyToAll
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
        return getUuid().compare(uuid) == 0;
    }

    bool
    Scene::hasName
    (string name)
    {
        return getName().compare(name) == 0;
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
    Scene::getProjectHandle
    ()
    {
        return mProjectHandle;
    }

    bool
    Scene::isState
    (SceneState state)
    {
        return mState == state;
    }

    Transform3D
    Scene::getDefaultCameraTransform
    ()
    {
        Transform3D defaultCameraTransform;
        nlohmann::json camera = mJson[Constants::SCENE_JSON_CAMERA];

        if (!camera.is_null())
        {
            nlohmann::json translation = camera[Constants::SCENE_JSON_TRANSLATION];
            defaultCameraTransform.setTranslation(
                translation[Constants::SCENE_JSON_X],
                translation[Constants::SCENE_JSON_Y],
                translation[Constants::SCENE_JSON_Z]
            );
            nlohmann::json rotation = camera[Constants::SCENE_JSON_ROTATION];
            defaultCameraTransform.setRotation(
                rotation[Constants::SCENE_JSON_X],
                rotation[Constants::SCENE_JSON_Y],
                rotation[Constants::SCENE_JSON_Z]
            );
        }

        return defaultCameraTransform;
    }

} // End of Dream

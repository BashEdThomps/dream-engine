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

#include <functional>
#include <algorithm>

#include "../Project/Project.h"

namespace Dream
{
    SceneRuntime Scene::getRuntime() const
    {
        return mRuntime;
    }

    Scene::Scene
    (Project* project, nlohmann::json json)
        : mProjectHandle(project),
          mJson(json)
    {
        mRuntime.setState(NOT_LOADED);
        nlohmann::json sceneObjects = mJson[Constants::SCENE_SCENE_OBJECTS];

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
                    mRootSceneObject.reset(nextSceneObject);
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
        gravity[Constants::X_INDEX] = mJson[Constants::SCENE_GRAVITY][Constants::X];
        gravity[Constants::Y_INDEX] = mJson[Constants::SCENE_GRAVITY][Constants::Y];
        gravity[Constants::Z_INDEX] = mJson[Constants::SCENE_GRAVITY][Constants::Z];
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
        return mJson[Constants::SCENE_NAME];
    }

    string
    Scene::getUuid
    ()
    {
       return mJson[Constants::SCENE_UUID];
    }

    void
    Scene::setUuid
    (string uuid)
    {
        mJson[Constants::SCENE_UUID] = uuid;
    }

    void
    Scene::setName
    (string name)
    {
        mJson[Constants::SCENE_NAME] = name ;
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
        if (!mRootSceneObject)
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
        mRootSceneObject.reset(root);
    }

    SceneObject*
    Scene::getRootSceneObject
    ()
    {
        return mRootSceneObject.get();
    }

    void
    Scene::setCameraMovementSpeed
    (float speed)
    {
        mJson[Constants::SCENE_CAMERA][Constants::SCENE_MOVEMENT_SPEED] = speed;
    }

    float
    Scene::getCameraMovementSpeed
    ()
    {
        return mJson[Constants::SCENE_CAMERA][Constants::SCENE_MOVEMENT_SPEED];
    }

    vector<float>
    Scene::getAmbientLightColour
    ()
    {
        vector<float> ambientColour;
        ambientColour.reserve(4);
        ambientColour[Constants::RED_INDEX] = mJson[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::RED];
        ambientColour[Constants::GREEN_INDEX] = mJson[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::GREEN];
        ambientColour[Constants::BLUE_INDEX] = mJson[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::BLUE];
        ambientColour[Constants::ALPHA_INDEX] = mJson[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::ALPHA];
        return ambientColour;
    }

    vector<float>
    Scene::getClearColour
    ()
    {
        vector<float> clearColour;
        clearColour.reserve(4);
        clearColour[Constants::RED_INDEX] = mJson[Constants::SCENE_CLEAR_COLOUR][Constants::RED];
        clearColour[Constants::GREEN_INDEX] = mJson[Constants::SCENE_CLEAR_COLOUR][Constants::GREEN];
        clearColour[Constants::BLUE_INDEX] = mJson[Constants::SCENE_CLEAR_COLOUR][Constants::BLUE];
        clearColour[Constants::ALPHA_INDEX] = mJson[Constants::SCENE_CLEAR_COLOUR][Constants::ALPHA];
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
        mRuntime.getDeleteQueue().push_back(object);
    }

    void
    Scene::clearDeleteQueue
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "Scene: clearDeleteQueue" << endl;
        }
        mRuntime.getDeleteQueue().clear();
    }

    void
    Scene::destroyDeleteQueue
    ()
    {
        if (!mRuntime.getDeleteQueue().empty())
        {
            for(SceneObject* obj : mRuntime.getDeleteQueue())
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
        return mRuntime.getDeleteQueue();
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

        mRuntime.setState(LOADED);
    }
    void
    Scene::findDeleteFlaggedScripts
    ()
    {
        if (Constants::VERBOSE)
        {
            cout << "Scene: Cleanup Deleted Scripts Called" << endl;
        }
        vector<SceneObject*> objects = mRuntime.getDeleteQueue();

        for (SceneObject* it : objects)
        {
            mProjectHandle->getRuntime()->getLuaEngine()->removeFromScriptMap(it);
        }
    }

    bool
    Scene::getPhysicsDebug
    ()
    {
        return mJson[Constants::SCENE_PHYSICS_DEBUG];
    }

    nlohmann::json
    Scene::getJson
    ()
    {
        return mJson;
    }

    string
    Scene::getNotes
    ()
    {
        return mJson[Constants::SCENE_NOTES];
    }

    void
    Scene::setNotes
    (string notes)
    {
        mJson[Constants::SCENE_NOTES] = notes;
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

        if(mRuntime.getState() == DONE)
        {
           cleanUpSceneObjects();
           flush();
           mRuntime.setState(CLEANED_UP);
        }
        else
        {
            cerr << "Scene: Cannot cleanUp Scene "
                 << getNameAndUuidString()
                 << " State != DONE"
                 << endl;
        }
    }

    Project*
    Scene::getProjectHandle
    ()
    {
        return mProjectHandle;
    }

    Transform3D
    Scene::getDefaultCameraTransform
    ()
    {
        Transform3D defaultCameraTransform;
        nlohmann::json camera = mJson[Constants::SCENE_CAMERA];

        if (!camera.is_null())
        {
            nlohmann::json translation = camera[Constants::SCENE_TRANSLATION];
            defaultCameraTransform.setTranslation(
                translation[Constants::X],
                translation[Constants::Y],
                translation[Constants::Z]
            );
            nlohmann::json rotation = camera[Constants::SCENE_ROTATION];
            defaultCameraTransform.setRotation(
                rotation[Constants::X],
                rotation[Constants::Y],
                rotation[Constants::Z]
            );
        }

        return defaultCameraTransform;
    }

} // End of Dream

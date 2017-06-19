/*
 * SceneRuntime.cpp
 *
 * Created: 15 2017 by Ashley
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

#include "SceneRuntime.h"

#include <iostream>

#include "../Common/Constants.h"

#include "../Project/ProjectRuntime.h"

#include "SceneDefinition.h"

#include "SceneObject/SceneObjectDefinition.h"
#include "SceneObject/SceneObjectRuntime.h"

#include "../Lua/LuaEngine.h"

using std::cout;
using std::cerr;

namespace Dream
{
    SceneRuntime::SceneRuntime
    (ProjectRuntime* project)
        : // Init list
          Runtime(),
          mGravity({0,0,0}),
          mClearColour({0,0,0,0}),
          mAmbientColour({0,0,0}),
          mProjectRuntimeHandle(project)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneRuntime: Constructing " << endl;
        }

    }

    SceneRuntime::~SceneRuntime
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "SceneRuntime: Destructing " << endl;
        }
    }

    SceneState
    SceneRuntime::getState
    ()
    {
        return mState;
    }

    void
    SceneRuntime::setState
    (SceneState state)
    {
        mState = state;
    }

    vector<float>
    SceneRuntime::getGravity
    ()
    {
        return mGravity;
    }

    void
    SceneRuntime::setGravity
    (vector<float> gravity)
    {
        mGravity = gravity;
    }

    vector<float>
    SceneRuntime::getClearColour
    ()
    {
        return mClearColour;
    }

    void
    SceneRuntime::setClearColour
    (vector<float> clearColour)
    {
        mClearColour = clearColour;
    }

    vector<float>
    SceneRuntime::getAmbientColour
    ()
    {
        return mAmbientColour;
    }

    void
    SceneRuntime::setAmbientColour
    (vector<float> ambientColour)
    {
        mAmbientColour = ambientColour;
    }

    vector<SceneObjectRuntime*>
    SceneRuntime::getSceneObjectRuntimeDeleteQueue
    ()
    {
        return mSceneObjectRuntimeDeleteQueue;
    }

    void
    SceneRuntime::addSceneObjectRuntimeToDeleteQueue
    (SceneObjectRuntime* object)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneRuntime: Adding "
                 << object->getNameAndUuidString()
                 << " to SceneObjectRuntime delete queue" << endl;
        }
       mSceneObjectRuntimeDeleteQueue.push_back(object);
    }

    void
    SceneRuntime::clearSceneObjectRuntimeDeleteQueue
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "SceneRuntime: clearSceneObjectRuntimeDeleteQueue" << endl;
        }
        mSceneObjectRuntimeDeleteQueue.clear();
    }

    void
    SceneRuntime::destroySceneObjectRuntimeDeleteQueue
    ()
    {
        if (!mSceneObjectRuntimeDeleteQueue.empty())
        {
            for(SceneObjectRuntime* obj : mSceneObjectRuntimeDeleteQueue)
            {
                obj->cleanUp();
            }
        }
        clearSceneObjectRuntimeDeleteQueue();
    }

    SceneObjectRuntime*
    SceneRuntime::getSceneObjectRuntimeHandleByUuid
    (string uuid)
    {
        return static_cast<SceneObjectRuntime*>
        (
            mRootSceneObjectRuntime->applyToAll
            (
                function<void*(SceneObjectRuntime*)>
                (
                    [&](SceneObjectRuntime* currentRuntime)
                    {
                        if (currentRuntime->hasUuid(uuid))
                        {
                            return currentRuntime;
                        }
                        return static_cast<SceneObjectRuntime*>(nullptr);
                    }
                )
            )
        );
    }

    SceneObjectRuntime*
    SceneRuntime::getSceneObjectRuntimeHandleByName
    (string name)
    {
        return static_cast<SceneObjectRuntime*>
        (
            mRootSceneObjectRuntime->applyToAll
            (
                function<void*(SceneObjectRuntime*)>
                (
                    [&](SceneObjectRuntime* currentRuntime)
                    {
                        if (currentRuntime->hasName(name))
                        {
                            return currentRuntime;
                        }
                        return static_cast<SceneObjectRuntime*>(nullptr);
                    }
                )
            )
        );
    }

    int
    SceneRuntime::countSceneObjectRuntimes
    ()
    {
        int count = 0;
        mRootSceneObjectRuntime->applyToAll
        (
            function<void*(SceneObjectRuntime*)>
            (
                [&](SceneObjectRuntime*)
                {
                    count++;
                    return nullptr;
                }
            )
        );
        return count;
    }

    void
    SceneRuntime::showScenegraph
    ()
    {
        if (!mRootSceneObjectRuntime)
        {
            cout << "SceneRuntime: Scenegraph is empty (no root SceneObjectRuntime)" << endl;
            return;
        }

        mRootSceneObjectRuntime->applyToAll
        (
            function<void*(SceneObjectRuntime*)>
            (
                [&](SceneObjectRuntime* obj)
                {
                        cout << "SceneObjectRuntime: showScenegraph not implemented" << endl;
                    //obj->showStatus();
                    return nullptr;
                }
            )
        );
    }

    void
    SceneRuntime::setRootSceneObjectRuntime
    (SceneObjectRuntime* root)
    {
        mRootSceneObjectRuntime.reset(root);
    }

    SceneObjectRuntime*
    SceneRuntime::getRootSceneObjectRuntimeHandle
    ()
    {
        return mRootSceneObjectRuntime.get();
    }

    void
    SceneRuntime::findDeleteFlaggedSceneObjectRuntimes
    ()
    {
        if (Constants::VERBOSE)
        {
            cout << "SceneRuntime: findDeleteFlaggedSceneObjects Called" << endl;
        }

        mRootSceneObjectRuntime->applyToAll
        (
            function<void*(SceneObjectRuntime*)>
            (
                [&](SceneObjectRuntime* obj)
                {
                    if (obj->getDeleteFlag())
                    {
                        addSceneObjectRuntimeToDeleteQueue(obj);
                    }
                    return nullptr;
                }
            )
        );
    }

    void
    SceneRuntime::createAllAssetInstances
    ()
    {
        if (Constants::VERBOSE)
        {
            cout << "SceneRuntime: Create All Asset Instances Called" << endl;
        }

        mRootSceneObjectRuntime->applyToAll
        (
            function<void*(SceneObjectRuntime*)>
            (
                [&](SceneObjectRuntime* sceneObjectRuntime)
                {
                    // Not loaded && not marked to delete
                    if (!sceneObjectRuntime->getLoadedFlag() && !sceneObjectRuntime->getDeleteFlag())
                    {
                        sceneObjectRuntime->createAssetInstances();
                    }
                    return nullptr;
                }
            )
        );
    }

    void
    SceneRuntime::loadAllAssetInstances
    ()
    {
        mRootSceneObjectRuntime->applyToAll
        (
            function<void*(SceneObjectRuntime*)>
            (
                [&](SceneObjectRuntime* sceneObjectRuntime)
                {
                    // Not loaded && not marked to delete
                    if (!sceneObjectRuntime->getLoadedFlag())
                    {
                        if(!sceneObjectRuntime->getDeleteFlag())
                        {
                            sceneObjectRuntime->loadAssetInstances();
                        }
                    }
                    return nullptr;
                }
            )
        );

        setState(SCENE_STATE_LOADED);
    }
    void
    SceneRuntime::findDeleteFlaggedScripts
    ()
    {
        if (Constants::VERBOSE)
        {
            cout << "SceneRuntime: Cleanup Deleted Scripts Called" << endl;
        }
        vector<SceneObjectRuntime*> objects = getSceneObjectRuntimeDeleteQueue();

        for (SceneObjectRuntime* runtime : objects)
        {
            mProjectRuntimeHandle->getLuaEngineHandle()->removeFromScriptMap(runtime);
        }
    }

    void
    SceneRuntime::cleanUpSceneObjectRuntimes
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "SceneRuntime: cleanUpSceneObjectRuntimes" << endl;
        }

        mRootSceneObjectRuntime->applyToAll
        (
            function<void*(SceneObjectRuntime*)>
            (
                [&](SceneObjectRuntime* objectRuntime)
                {
                    objectRuntime->setDeleteFlag(true);
                    return nullptr;
                }
            )
        );
    }

    void
    SceneRuntime::flush
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "SceneRuntime: flush" << endl;
        }
        findDeleteFlaggedSceneObjectRuntimes();
        findDeleteFlaggedScripts();
        destroySceneObjectRuntimeDeleteQueue();
    }

    void
    SceneRuntime::cleanUp
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "SceneRuntime: cleanUp called on "
                 <<  getNameAndUuidString()
                 << endl;
        }

        if(getState() == SCENE_STATE_DONE)
        {
           cleanUpSceneObjectRuntimes();
           flush();
           setState(SCENE_STATE_CLEANED_UP);
        }
        else
        {
            cerr << "SceneRuntime: Cannot cleanUp Scene "
                 << getNameAndUuidString()
                 << " State != DONE"
                 << endl;
        }
    }

    ProjectRuntime*
    SceneRuntime::getProjectRuntimeHandle
    ()
    {
        return mProjectRuntimeHandle;
    }

    bool
    SceneRuntime::hasRootSceneObjectRuntime
    ()
    {
        return mRootSceneObjectRuntime != nullptr;
    }

    void
    SceneRuntime::useDefinition
    (IDefinition* iDefinitionHandle)
    {
        SceneDefinition *sceneDefinitionHandle = dynamic_cast<SceneDefinition*>(iDefinitionHandle);

        if (Constants::DEBUG)
        {
            cout << "SceneRuntime: Using SceneDefinition "
                 << sceneDefinitionHandle->getNameAndUuidString() << endl;
        }

        // Assign Runtime attributes from Definition
        setName(sceneDefinitionHandle->getName());
        setUuid(sceneDefinitionHandle->getUuid());
        setAmbientColour(sceneDefinitionHandle->getAmbientColour());
        setClearColour(sceneDefinitionHandle->getClearColour());
        setGravity(sceneDefinitionHandle->getGravity());
        setPhysicsDebug(sceneDefinitionHandle->getPhysicsDebug());
        setCameraTransform(sceneDefinitionHandle->getCameraTransform());
        setCameraMovementSpeed(sceneDefinitionHandle->getCameraMovementSpeed());

        // Create Root SceneObjectRuntime
        setRootSceneObjectRuntime(new SceneObjectRuntime(this));
        mRootSceneObjectRuntime->useDefinition(sceneDefinitionHandle->getRootSceneObjectDefinitionHandle());

    }

    bool
    SceneRuntime::getPhysicsDebug
    ()
    {
        return mPhysicsDebug;
    }

    void
    SceneRuntime::setPhysicsDebug
    (bool physicsDebug)
    {
        mPhysicsDebug = physicsDebug;
    }

    float
    SceneRuntime::getCameraMovementSpeed
    ()
    {
        return mCameraMovementSpeed;
    }

    void
    SceneRuntime::setCameraMovementSpeed
    (float cameraMovementSpeed)
    {
        mCameraMovementSpeed = cameraMovementSpeed;
    }

    Transform3D
    SceneRuntime::getCameraTransform
    ()
    {
        return mCameraTransform;
    }

    void
    SceneRuntime::setCameraTransform
    (Transform3D cameraTransform)
    {
        mCameraTransform = cameraTransform;
    }
} // End of Dream

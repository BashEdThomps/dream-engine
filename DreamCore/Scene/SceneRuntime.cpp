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

#include "../Components/Graphics/GraphicsComponent.h"
#include "../Components/Graphics/Camera.h"
#include "../Components/Physics/PhysicsComponent.h"
#include "..//Components/Lua/LuaComponent.h"



namespace Dream
{
    SceneRuntime::SceneRuntime
    (SceneDefinition* sdHandle, ProjectRuntime* project)
        : // Init list
          Runtime(sdHandle),
          ILoggable ("SceneRuntime"),
          mGravity({0,0,0}),
          mClearColour({0,0,0,0}),
          mAmbientColour({0,0,0}),
          mProjectRuntimeHandle(project)
    {
        auto log = getLog();
        log->trace( "Constructing " );
        useDefinition(mDefinitionHandle);

    }

    SceneRuntime::~SceneRuntime
    ()
    {
        auto log = getLog();
        log->trace( "Destructing " );
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

    SceneObjectRuntime*
    SceneRuntime::getSceneObjectRuntimeHandleByUuid
    (string uuid)
    {
        return static_cast<SceneObjectRuntime*>(mRootSceneObjectRuntime->applyToAll(
            function<void*(SceneObjectRuntime*)>([&](SceneObjectRuntime* currentRuntime){
                if (currentRuntime->hasUuid(uuid)){
                    return currentRuntime;
                }
                return static_cast<SceneObjectRuntime*>(nullptr);
            }
        )));
    }

    SceneObjectRuntime*
    SceneRuntime::getSceneObjectRuntimeHandleByName
    (string name)
    {
        return static_cast<SceneObjectRuntime*>(
            mRootSceneObjectRuntime->applyToAll(function<void*(SceneObjectRuntime*)>([&](SceneObjectRuntime* currentRuntime){
                if (currentRuntime->hasName(name)){
                    return currentRuntime;
                }
                return static_cast<SceneObjectRuntime*>(nullptr);
            }
        )));
    }

    int
    SceneRuntime::countSceneObjectRuntimes
    ()
    {
        int count = 0;
        mRootSceneObjectRuntime->applyToAll(function<void*(SceneObjectRuntime*)>(
            [&](SceneObjectRuntime*){
            count++;
            return nullptr;
        }));
        return count;
    }

    void
    SceneRuntime::showScenegraph
    ()
    {
        auto log = getLog();
        if (!mRootSceneObjectRuntime)
        {
            log->info( "Scenegraph is empty (no root SceneObjectRuntime)" );
            return;
        }

        mRootSceneObjectRuntime->applyToAll(function<void*(SceneObjectRuntime*)>([&](SceneObjectRuntime* obj){
            log->info("showScenegraph not implemented");
            //obj->showStatus();
            return nullptr;
    }));
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
    SceneRuntime::collectGarbage
    ()
    {
        auto log = getLog();
        log->info( "Collecting Garbage {}" , getNameAndUuidString() );
        mRootSceneObjectRuntime->applyToAll(function<void*(SceneObjectRuntime*)>([&](SceneObjectRuntime* runt){
            runt->collectGarbage();
            return nullptr;
        }));
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
        auto log = getLog();
        SceneDefinition *sceneDefinitionHandle = dynamic_cast<SceneDefinition*>(iDefinitionHandle);

        log->info( "Using SceneDefinition ",  sceneDefinitionHandle->getNameAndUuidString() );

        // Assign Runtime attributes from Definition
        setName(sceneDefinitionHandle->getName());
        setUuid(sceneDefinitionHandle->getUuid());
        setAmbientColour(sceneDefinitionHandle->getAmbientColour());
        setClearColour(sceneDefinitionHandle->getClearColour());
        setGravity(sceneDefinitionHandle->getGravity());
        setPhysicsDebug(sceneDefinitionHandle->getPhysicsDebug());
        setCameraTransform(sceneDefinitionHandle->getCameraTransform());
        setCameraMovementSpeed(sceneDefinitionHandle->getCameraMovementSpeed());

        mProjectRuntimeHandle->getGraphicsComponentHandle()->setActiveSceneRuntime(this);
        mProjectRuntimeHandle->getPhysicsComponentHandle()->setGravity(getGravity());
        mProjectRuntimeHandle->getPhysicsComponentHandle()->setDebug(getPhysicsDebug());
        mProjectRuntimeHandle->getCameraHandle()->setTransform(getCameraTransform());
        mProjectRuntimeHandle->getCameraHandle()->setMovementSpeed(getCameraMovementSpeed());

        // Create Root SceneObjectRuntime
        SceneObjectDefinition* sodHandle = sceneDefinitionHandle->getRootSceneObjectDefinitionHandle();
        setRootSceneObjectRuntime(new SceneObjectRuntime(sodHandle,this));

        setState(SCENE_STATE_LOADED);
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

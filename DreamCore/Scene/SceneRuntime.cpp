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
#include "../Components/Scripting/IScriptComponent.h"



namespace Dream
{
    SceneRuntime::SceneRuntime
    (
        SceneDefinition* sd,
        ProjectRuntime* project
    ) :   IRuntime(sd),
          mGravity({0,0,0}),
          mClearColour({0,0,0,0}),
          mAmbientColour({0,0,0}),
          mProjectRuntime(project)
    {

        setLogClassName("SceneRuntime");
        auto log = getLog();
        log->trace( "Constructing " );
    }

    SceneRuntime::~SceneRuntime
    ()
    {
        auto log = getLog();
        log->trace( "Destructing " );

        if (mRootSceneObjectRuntime != nullptr)
        {
            delete mRootSceneObjectRuntime;
            mRootSceneObjectRuntime = nullptr;
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

    SceneObjectRuntime*
    SceneRuntime::getSceneObjectRuntimeByUuid
    (string uuid)
    {
        return mRootSceneObjectRuntime->applyToAll
        (
            function<SceneObjectRuntime*(SceneObjectRuntime*)>
            ([&](SceneObjectRuntime* currentRuntime)
            {
                if (currentRuntime->hasUuid(uuid))
                {
                    return currentRuntime;
                }
            return static_cast<SceneObjectRuntime*>(nullptr);
            }
        ));
    }

    SceneObjectRuntime*
    SceneRuntime::getSceneObjectRuntimeByName
    (string name)
    {
        return mRootSceneObjectRuntime->applyToAll
        (
            function<SceneObjectRuntime*(SceneObjectRuntime*)>
            ([&](SceneObjectRuntime* currentRuntime)
            {
                if (currentRuntime->hasName(name))
                {
                    return currentRuntime;
                }
            return static_cast<SceneObjectRuntime*>(nullptr);
            }
        ));
    }

    int
    SceneRuntime::countSceneObjectRuntimes
    ()
    {
        int count = 0;
        mRootSceneObjectRuntime->applyToAll(
            function<SceneObjectRuntime*(SceneObjectRuntime*)>
            ([&](SceneObjectRuntime*)
            {
                count++;
                return static_cast<SceneObjectRuntime*>(nullptr);
            }
        ));
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

        mRootSceneObjectRuntime->applyToAll(
            function<SceneObjectRuntime*(SceneObjectRuntime*)>
            ([&](SceneObjectRuntime*)
            {
                log->info("showScenegraph not implemented");
                //obj->showStatus();
                return nullptr;
            }
        ));
    }

    void
    SceneRuntime::setRootSceneObjectRuntime
    (SceneObjectRuntime* root)
    {
        mRootSceneObjectRuntime = root;
    }

    SceneObjectRuntime*
    SceneRuntime::getRootSceneObjectRuntime
    ()
    {
        return mRootSceneObjectRuntime;
    }

    void
    SceneRuntime::collectGarbage
    ()
    {
        auto log = getLog();
        log->info( "Collecting Garbage {}" , getNameAndUuidString() );
        mRootSceneObjectRuntime->applyToAll(
            function<SceneObjectRuntime*(SceneObjectRuntime*)>
            ([&](SceneObjectRuntime* runt)
            {
                runt->collectGarbage();
                return static_cast<SceneObjectRuntime*>(nullptr);
            }
        ));
    }

    ProjectRuntime*
    SceneRuntime::getProjectRuntime
    ()
    {
        return mProjectRuntime;
    }

    bool
    SceneRuntime::hasRootSceneObjectRuntime
    ()
    {
        return mRootSceneObjectRuntime != nullptr;
    }

    void
    SceneRuntime::useDefinition
    ()
    {
        auto log = getLog();

        auto sceneDefinition = dynamic_cast<SceneDefinition*>(mDefinition);

        if (sceneDefinition == nullptr)
        {
            return;
        }

        log->info( "Using SceneDefinition ",  sceneDefinition->getNameAndUuidString() );

        // Assign Runtime attributes from Definition
        setName(sceneDefinition->getName());
        setUuid(sceneDefinition->getUuid());
        setAmbientColour(sceneDefinition->getAmbientColour());
        setClearColour(sceneDefinition->getClearColour());
        setGravity(sceneDefinition->getGravity());
        setPhysicsDebug(sceneDefinition->getPhysicsDebug());
        setCameraTranslation(sceneDefinition->getCameraTranslation());
        setCameraPitch(sceneDefinition->getCameraPitch());
        setCameraYaw(sceneDefinition->getCameraYaw());
        setCameraMovementSpeed(sceneDefinition->getCameraMovementSpeed());

        // Propogate to project where required
        auto gfx = mProjectRuntime->getGraphicsComponent();
        if (gfx == nullptr)
        {
            log->error("Graphics Component is null");
        }
        gfx->setActiveSceneRuntime(this);

        auto physics = mProjectRuntime->getPhysicsComponent();
        if (physics != nullptr)
        {
            physics->setGravity(getGravity());
            physics->setDebug(getPhysicsDebug());
        }

        auto camera = mProjectRuntime->getCamera();

        if (camera != nullptr)
        {
            camera->setTranslation(getCameraTranslation());
            camera->setPitch(getCameraPitch());
            camera->setYaw(getCameraYaw());
            camera->updateCameraVectors();
            camera->setMovementSpeed(getCameraMovementSpeed());
        }

        // Create Root SceneObjectRuntime
        auto sod = sceneDefinition->getRootSceneObjectDefinition();
        auto sor = new SceneObjectRuntime(sod,this);
        sor->useDefinition();
        setRootSceneObjectRuntime(sor);
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

    glm::vec3 SceneRuntime::getCameraLookAt()
    {
       return mCameraLookAt;
    }

    void SceneRuntime::setCameraLookAt(glm::vec3 lookAt)
    {
        mCameraLookAt = lookAt;
    }

    float SceneRuntime::getCameraPitch()
    {
        return mCameraPitch;
    }

    void SceneRuntime::setCameraPitch(float pitch)
    {
       mCameraPitch = pitch;
    }

    float SceneRuntime::getCameraYaw()
    {
       return mCameraYaw;
    }

    void SceneRuntime::setCameraYaw(float yaw)
    {
       mCameraYaw = yaw;
    }

    vec3
    SceneRuntime::getCameraTranslation
    ()
    {
        return mCameraTranslation;
    }

    void
    SceneRuntime::setCameraTranslation
    (vec3 cameraTransform)
    {
        mCameraTranslation = cameraTransform;
    }

} // End of Dream

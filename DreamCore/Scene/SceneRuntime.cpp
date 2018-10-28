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

#include "../Project/ProjectRuntime.h"

#include "SceneDefinition.h"

#include "SceneObject/SceneObjectDefinition.h"
#include "SceneObject/SceneObjectRuntime.h"

#include "../Components/Graphics/GraphicsComponent.h"
#include "../Components/Graphics/Shader/ShaderCache.h"
#include "../Components/Graphics/Shader/ShaderInstance.h"
#include "../Components/Graphics/Camera.h"

#include "../Components/Physics/PhysicsComponent.h"

#include "../Components/Scripting/IScriptComponent.h"



namespace Dream
{
    SceneRuntime::SceneRuntime
    (
        SceneDefinition* sd,
        ProjectRuntime* project
    ) : IRuntime(sd, sd->getName(),sd->getUuid()),
        mClearColour({0,0,0,0}),
        mAmbientColour({0,0,0}),
        mProjectRuntime(project),
        mRootSceneObjectRuntime(nullptr)
    {

        setLogClassName("SceneRuntime");
        auto log = getLog();
        log->trace( "Constructing " );
    }

    SceneRuntime::~SceneRuntime
    ()
    {
        auto log = getLog();
        log->trace("Destructing");

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
        if (mProjectRuntime)
        {
            return mProjectRuntime->getPhysicsComponent()->getGravity();
        }
        return vector<float>{0.0f,0.0f,0.0f};
    }

    void
    SceneRuntime::setGravity
    (vector<float> gravity)
    {
        if (mProjectRuntime)
        {
            mProjectRuntime->getPhysicsComponent()->setGravity(gravity);
        }
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
        );
    }

    SceneObjectRuntime*
    SceneRuntime::getSceneObjectRuntimeByName
    (string name)
    {
        return mRootSceneObjectRuntime->applyToAll
        (
            function<SceneObjectRuntime*(SceneObjectRuntime*)>
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
        );
    }

    int
    SceneRuntime::countSceneObjectRuntimes
    ()
    {
        int count = 0;
        mRootSceneObjectRuntime->applyToAll
        (
            function<SceneObjectRuntime*(SceneObjectRuntime*)>
            (
                [&](SceneObjectRuntime*)
                {
                    count++;
                    return static_cast<SceneObjectRuntime*>(nullptr);
                }
            )
        );
        return count;
    }

    void
    SceneRuntime::showScenegraph
    ()
    {
        auto log = getLog();
        if (!mRootSceneObjectRuntime)
        {
            log->debug( "Scenegraph is empty (no root SceneObjectRuntime)" );
            return;
        }

        mRootSceneObjectRuntime->applyToAll
        (
            function<SceneObjectRuntime*(SceneObjectRuntime*)>
            (
                [&](SceneObjectRuntime*)
                {
                    log->debug("showScenegraph not implemented");
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
        log->debug( "Collecting Garbage {}" , getNameAndUuidString() );
        mRootSceneObjectRuntime->applyToAll
        (
            function<SceneObjectRuntime*(SceneObjectRuntime*)>
            (
                [&](SceneObjectRuntime* runt)
                {
                    runt->collectGarbage();
                    return static_cast<SceneObjectRuntime*>(nullptr);
                }
            )
        );
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

    bool
    SceneRuntime::useDefinition
    ()
    {
        auto log = getLog();

        auto sceneDefinition = dynamic_cast<SceneDefinition*>(mDefinition);

        if (sceneDefinition == nullptr)
        {
            return false;
        }

        log->debug( "Using SceneDefinition ",  sceneDefinition->getNameAndUuidString() );

        // Assign Runtime attributes from Definition
        setName(sceneDefinition->getName());
        setUuid(sceneDefinition->getUuid());
        setAmbientColour(sceneDefinition->getAmbientColour());
        setClearColour(sceneDefinition->getClearColour());

        // Setup Camera
        mCameraHandle = mProjectRuntime->getCamera();
        if (mCameraHandle == nullptr)
        {
            log->error("Camera is null");
            return false;
        }
        mCameraHandle->setTranslation(sceneDefinition->getCameraTranslation());
        mCameraHandle->setMovementSpeed(sceneDefinition->getCameraMovementSpeed());
        mCameraHandle->setPitch(sceneDefinition->getCameraPitch());
        mCameraHandle->setYaw(sceneDefinition->getCameraYaw());

        // Setup Physics
        auto physics = mProjectRuntime->getPhysicsComponent();
        if (physics == nullptr)
        {
            log->error("Physics component is null");
            return false;
        }
        physics->setGravity(sceneDefinition->getGravity());
        physics->setDebug(sceneDefinition->getPhysicsDebug());

        // Load Lighting Shader
        auto shaderCache = mProjectRuntime->getShaderCache();
        auto shaderUuid = sceneDefinition->getLightingShader();
        mLightingShader = dynamic_cast<ShaderInstance*>(shaderCache->getInstance(shaderUuid));
        if (mLightingShader == nullptr)
        {
            log->error(
                        "Unable to load lighting shader {} for Scene {}",
                        shaderUuid,
                        getNameAndUuidString()
                        );
        }
        auto gfx = mProjectRuntime->getGraphicsComponent();
        if (gfx == nullptr)
        {
            log->error("Graphics Component is null");
            return false;
        }
        gfx->setLightingShader(mLightingShader);

        // Create Root SceneObjectRuntime
        auto sod = sceneDefinition->getRootSceneObjectDefinition();
        auto sor = new SceneObjectRuntime(sod,this);
        if (!sor->useDefinition())
        {
            log->error("Error using scene object runtime definition");
            delete sor;
            sor = nullptr;
            return false;
        }
        setRootSceneObjectRuntime(sor);
        setState(SCENE_STATE_LOADED);
        mProjectRuntime->getShaderCache()->logShaders();
        return true;
    }

    bool
    SceneRuntime::getPhysicsDebug
    ()
    {
        if (mProjectRuntime)
        {
            return mProjectRuntime->getPhysicsComponent()->getDebug();
        }
        return false;
    }

    void
    SceneRuntime::setPhysicsDebug
    (bool physicsDebug)
    {
        if (mProjectRuntime)
        {
            mProjectRuntime->getPhysicsComponent()->setDebug(physicsDebug);
        }
    }

    float
    SceneRuntime::getCameraMovementSpeed
    ()
    {
        return mCameraHandle->getMovementSpeed();
    }

    void
    SceneRuntime::setCameraMovementSpeed
    (float cameraMovementSpeed)
    {
        mCameraHandle->setMovementSpeed(cameraMovementSpeed);
    }

    glm::vec3 SceneRuntime::getCameraLookAt()
    {
        return mCameraHandle->getLookAt();
    }

    void SceneRuntime::setCameraLookAt(glm::vec3 lookAt)
    {
        mCameraHandle->setLookAt(lookAt);
    }

    float SceneRuntime::getCameraPitch()
    {
        return mCameraHandle->getPitch();
    }

    void SceneRuntime::setCameraPitch(float pitch)
    {
        mCameraHandle->setPitch(pitch);
    }

    float SceneRuntime::getCameraYaw()
    {
        return mCameraHandle->getYaw();
    }

    void SceneRuntime::setCameraYaw(float yaw)
    {
        mCameraHandle->setYaw(yaw);
    }

    vec3
    SceneRuntime::getCameraTranslation
    ()
    {
        return mCameraHandle->getTranslation();
    }

    void
    SceneRuntime::setCameraTranslation
    (vec3 tx)
    {
        mCameraHandle->setTranslation(tx);
    }

    ShaderInstance*
    SceneRuntime::getLightingShader
    () const
    {
        return mLightingShader;
    }

    void
    SceneRuntime::setLightingShader
    (ShaderInstance* lightingShader)
    {
        mLightingShader = lightingShader;
    }

    void
    SceneRuntime::setMeshCullDistance
    (float mcd)
    {
       if (mProjectRuntime)
       {
           auto gfx = mProjectRuntime->getGraphicsComponent();
           if (gfx)
           {
               gfx->setMeshCullDistance(mcd);
           }
       }
    }

    float
    SceneRuntime::getMeshCullDistance
    ()
    {
        if (mProjectRuntime)
        {
            auto gfx = mProjectRuntime->getGraphicsComponent();
            if (gfx)
            {
               return gfx->getMeshCullDistance();
            }
        }
        return 0.f;
    }

    void
    SceneRuntime::setMinDrawDistance
    (float f)
    {
       auto gfx = mProjectRuntime->getGraphicsComponent();
       if (gfx)
       {
           gfx->setMinimumDraw(f);
       }
    }

    void
    SceneRuntime::setMaxDrawDistance
    (float f)
    {
       auto gfx = mProjectRuntime->getGraphicsComponent();
       if (gfx)
       {
           gfx->setMaximumDraw(f);
       }
    }

} // End of Dream

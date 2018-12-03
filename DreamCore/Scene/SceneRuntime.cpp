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
#include "../Components/Scripting/ScriptComponent.h"
#include "../Components/Input/FlyingInputHandler.h"

namespace Dream
{
    SceneRuntime::SceneRuntime
    (
            SceneDefinition* sd,
            ProjectRuntime* project
            ) : IRuntime(sd),
        mState(SceneState::SCENE_STATE_TO_LOAD),
        mClearColour({0,0,0,0}),
        mAmbientColour({0,0,0}),
        mProjectRuntime(project),
        mRootSceneObjectRuntime(nullptr),
        mInputScript(nullptr),
        mNanoVGScript(nullptr),
        mMinDrawDistance(0.1f),
        mMaxDrawDistance(1000.0f),
        mMeshCullDistance(1000.0f)
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
        if (mState != SCENE_STATE_DESTROYED)
        {
            destroyRuntime();
        }
    }

    void
    SceneRuntime::destroyRuntime
    ()
    {
        getLog()->critical("Destroying runtime {}",getNameAndUuidString());

        if (mRootSceneObjectRuntime != nullptr)
        {
            delete mRootSceneObjectRuntime;
            mRootSceneObjectRuntime = nullptr;
        }

        mLightingPassShader = nullptr;
        mSceneObjectRuntimeCleanUpQueue.clear();
        mState = SceneState::SCENE_STATE_DESTROYED;
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
        if (state >= mState ||
            (mState == SCENE_STATE_ACTIVE && state == SCENE_STATE_LOADED))
        {
            mState = state;
        }
        else
        {
            getLog()->error("Cannot switch scene state from {} to {}",mState,state);
        }
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
        if (!mRootSceneObjectRuntime)
        {
            return nullptr;
        }

        return mRootSceneObjectRuntime->applyToAll
        (
            function<SceneObjectRuntime*(SceneObjectRuntime*)>
            (
                [&](SceneObjectRuntime* currentRuntime)
                {
                    if (!currentRuntime)
                    {
                        return static_cast<SceneObjectRuntime*>(nullptr);
                    }
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
        if (!mRootSceneObjectRuntime)
        {
            return nullptr;
        }
        return mRootSceneObjectRuntime->applyToAll
        (
            function<SceneObjectRuntime*(SceneObjectRuntime*)>
            (
                [&](SceneObjectRuntime* currentRuntime)
                {
                    if (!currentRuntime)
                    {
                        return static_cast<SceneObjectRuntime*>(nullptr);
                    }

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
        if (!mRootSceneObjectRuntime)
        {
            return 0;
        }
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
            log->error("SceneDefinition is null");
            return false;
        }

        log->debug( "Using SceneDefinition ",  sceneDefinition->getNameAndUuidString() );

        // Assign Runtime attributes from Definition
        setName(sceneDefinition->getName());
        setUuid(sceneDefinition->getUuid());
        setAmbientColour(sceneDefinition->getAmbientColour());
        setClearColour(sceneDefinition->getClearColour());

        // Setup Camera
        mCamera.setTranslation(sceneDefinition->getCameraTranslation());
        mCamera.setMovementSpeed(sceneDefinition->getCameraMovementSpeed());
        mCamera.setPitch(sceneDefinition->getCameraPitch());
        mCamera.setYaw(sceneDefinition->getCameraYaw());

        setMeshCullDistance(sceneDefinition->getMeshCullDistance());
        setMinDrawDistance(sceneDefinition->getMinDrawDistance());
        setMaxDrawDistance(sceneDefinition->getMaxDrawDistance());

        // Load Lighting Shader
        auto shaderCache = mProjectRuntime->getShaderCache();
        auto shaderUuid = sceneDefinition->getLightingPassShader();
        mLightingPassShader = dynamic_cast<ShaderInstance*>(shaderCache->getInstance(shaderUuid));
        shaderUuid = sceneDefinition->getShadowPassShader();
        mShadowPassShader = dynamic_cast<ShaderInstance*>(shaderCache->getInstance(shaderUuid));
        if (mLightingPassShader == nullptr)
        {
            log->error("Unable to load lighting shader {} for Scene {}",shaderUuid,getNameAndUuidString());
        }
        if (mShadowPassShader == nullptr)
        {
            log->error("Unable to load shadow shader {} for Scene {}",shaderUuid,getNameAndUuidString());
        }
        // Scripts
        auto scriptCache = mProjectRuntime->getScriptCache();
        auto inputScriptUuid = sceneDefinition->getInputScript();
        mInputScript = dynamic_cast<ScriptInstance*>(scriptCache->getInstance(inputScriptUuid));
        if (!mInputScript)
        {
            log->error("Unable to load Input Handler Script {}",inputScriptUuid);
        }
        mInputScript->registerInputScript();

        auto nvgScriptUuid = sceneDefinition->getNanoVGScript();
        mNanoVGScript = dynamic_cast<ScriptInstance*>(scriptCache->getInstance(nvgScriptUuid));
        if (!mNanoVGScript)
        {
            log->error("Unable to load NanoVG Script {}",nvgScriptUuid);
        }
        mNanoVGScript->registerNanoVGScript();

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

        // Physics
        mProjectRuntime->getPhysicsComponent()->setGravity(sceneDefinition->getGravity());
        setRootSceneObjectRuntime(sor);
        setState(SceneState::SCENE_STATE_LOADED);
        mProjectRuntime->getShaderCache()->logShaders();
        auto focused = getSceneObjectRuntimeByUuid(sceneDefinition->getCameraFocusedOn());
        mCamera.setFocusedSceneObejct(focused);

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

    ShaderInstance*
    SceneRuntime::getLightingPassShader
    () const
    {
        return mLightingPassShader;
    }

    void
    SceneRuntime::setLightingPassShader
    (ShaderInstance* lightingShader)
    {
        mLightingPassShader = lightingShader;
    }

    void
    SceneRuntime::setMeshCullDistance
    (float mcd)
    {
      mMeshCullDistance = mcd;
    }

    float
    SceneRuntime::getMeshCullDistance
    ()
    {
        return mMeshCullDistance;
    }

    void
    SceneRuntime::setMinDrawDistance
    (float f)
    {
        mMinDrawDistance = f;
    }

    float SceneRuntime::getMinDrawDistance() const
    {
        return mMinDrawDistance;
    }

    float SceneRuntime::getMaxDrawDistance() const
    {
        return mMaxDrawDistance;
    }

    vector<IAssetInstance*>
    SceneRuntime::getAssetInstances(AssetType t)
    {
        vector<IAssetInstance*> instances;
        if (mRootSceneObjectRuntime)
        {
            mRootSceneObjectRuntime->applyToAll
            (
                function<SceneObjectRuntime*(SceneObjectRuntime*)>
                (
                    [&](SceneObjectRuntime* currentRuntime)
                    {
                        IAssetInstance* inst = currentRuntime->getAssetInstance(t);
                        if (inst)
                        {
                            instances.push_back(inst);
                        }
                        return static_cast<SceneObjectRuntime*>(nullptr);
                    }
                )
            );
        }
        return instances;
    }

    vector<SceneObjectRuntime*>
    SceneRuntime::getSceneObjectsWithInstanceOf(IAssetDefinition* def)
    {
        vector<SceneObjectRuntime*> runtimes;
        if (mRootSceneObjectRuntime)
        {
            mRootSceneObjectRuntime->applyToAll
            (
                function<SceneObjectRuntime*(SceneObjectRuntime*)>
                (
                    [&](SceneObjectRuntime* currentRuntime)
                    {
                        IAssetInstance* inst = currentRuntime->getAssetInstance(def->getAssetType());
                        if (inst && inst->getUuid().compare(def->getUuid()) == 0)
                        {
                            runtimes.push_back(currentRuntime);
                        }
                        return static_cast<SceneObjectRuntime*>(nullptr);
                    }
                )
            );
        }
        return runtimes;
    }


    void
    SceneRuntime::setMaxDrawDistance
    (float f)
    {
        mMaxDrawDistance = f;
    }

    Camera*
    SceneRuntime::getCamera
    ()
    {
        return &mCamera;
    }

    ShaderInstance*
    SceneRuntime::getShadowPassShader
    ()
    const
    {
        return mShadowPassShader;
    }

    void
    SceneRuntime::setShadowPassShader
    (ShaderInstance* shadowPassShader)
    {
        mShadowPassShader = shadowPassShader;
    }

    ScriptInstance*
    SceneRuntime::getInputScript
    ()
    const
    {
        return mInputScript;
    }

    ScriptInstance*
    SceneRuntime::getNanoVGScript
    ()
    const
    {
        return mNanoVGScript;
    }

} // End of Dream

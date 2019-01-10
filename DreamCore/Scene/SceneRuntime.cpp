/*
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
#include "../Components/Graphics/Shader/ShaderRuntime.h"
#include "../Components/Graphics/Camera.h"

#include "../Components/Physics/PhysicsComponent.h"
#include "../Components/Scripting/ScriptComponent.h"

namespace Dream
{
    SceneRuntime::SceneRuntime
    (SceneDefinition* sd, ProjectRuntime* project)
        : Runtime(sd),
        mState(SceneState::SCENE_STATE_TO_LOAD),
        mCamera(Camera(this)),
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
#ifdef DREAM_LOG
        setLogClassName("SceneRuntime");
        auto log = getLog();
        log->trace( "Constructing " );
#endif
    }

    SceneRuntime::~SceneRuntime
    ()
    {
#ifdef DREAM_LOG
        auto log = getLog();
        log->trace("Destructing");
#endif
        if (mState != SCENE_STATE_DESTROYED)
        {
            destroyRuntime();
        }
    }

    void
    SceneRuntime::destroyRuntime
    ()
    {
#ifdef DREAM_LOG
        getLog()->critical("Destroying runtime {}",getNameAndUuidString());
#endif

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
        if (state >= mState || (mState == SCENE_STATE_ACTIVE && state == SCENE_STATE_LOADED))
        {
            mState = state;
        }
        else
        {
#ifdef DREAM_LOG
            getLog()->error("Cannot switch scene state from {} to {}",mState,state);
#endif
        }
    }

    const vector<float>
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
    (const vector<float>& gravity)
    {
        if (mProjectRuntime)
        {
            mProjectRuntime->getPhysicsComponent()->setGravity(gravity);
        }
    }

    const vector<float>&
    SceneRuntime::getClearColour
    ()
    {
        return mClearColour;
    }

    void
    SceneRuntime::setClearColour
    (const vector<float>& clearColour)
    {
        mClearColour = clearColour;
    }

    const vector<float>&
    SceneRuntime::getAmbientColour
    ()
    {
        return mAmbientColour;
    }

    void
    SceneRuntime::setAmbientColour
    (const vector<float>& ambientColour)
    {
        mAmbientColour = ambientColour;
    }

    SceneObjectRuntime*
    SceneRuntime::getSceneObjectRuntimeByUuid
    (uint32_t uuid)
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
    (const string& name)
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

#ifdef DREAM_LOG
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
#endif

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

#ifdef DREAM_LOG
        auto log = getLog();
        log->debug( "Collecting Garbage {}" , getNameAndUuidString() );
#endif
        mRootSceneObjectRuntime->applyToAll
        (
            function<SceneObjectRuntime*(SceneObjectRuntime*)>
            (
                [=](SceneObjectRuntime* runt)
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
#ifdef DREAM_LOG
        auto log = getLog();
#endif
        auto sceneDefinition = dynamic_cast<SceneDefinition*>(mDefinition);

        if (sceneDefinition == nullptr)
        {
#ifdef DREAM_LOG
            log->error("SceneDefinition is null");
#endif
            return false;
        }

#ifdef DREAM_LOG
        log->debug( "Using SceneDefinition ",  sceneDefinition->getNameAndUuidString() );
#endif

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
        mLightingPassShader = dynamic_cast<ShaderRuntime*>(shaderCache->getInstance(shaderUuid));
        shaderUuid = sceneDefinition->getShadowPassShader();
        mShadowPassShader = dynamic_cast<ShaderRuntime*>(shaderCache->getInstance(shaderUuid));

#ifdef DREAM_LOG
        if (mLightingPassShader == nullptr)
        {
            log->error("Unable to load lighting shader {} for Scene {}",shaderUuid,getNameAndUuidString());
        }

        if (mShadowPassShader == nullptr)
        {
            log->error("Unable to load shadow shader {} for Scene {}",shaderUuid,getNameAndUuidString());
        }
#endif

        // Scripts
        auto scriptCache = mProjectRuntime->getScriptCache();
        auto inputScriptUuid = sceneDefinition->getInputScript();
        mInputScript = dynamic_cast<ScriptRuntime*>(scriptCache->getInstance(inputScriptUuid));
        if (!mInputScript)
        {
#ifdef DREAM_LOG
            log->error("Unable to load Input r Script {}",inputScriptUuid);
#endif
        }
        else
        {
            mInputScript->registerInputScript();
        }

        auto nvgScriptUuid = sceneDefinition->getNanoVGScript();
        mNanoVGScript = dynamic_cast<ScriptRuntime*>(scriptCache->getInstance(nvgScriptUuid));
        if (!mNanoVGScript)
        {
#ifdef DREAM_LOG
            log->error("Unable to load NanoVG Script {}",nvgScriptUuid);
#endif
        }
        else
        {
            mNanoVGScript->registerNanoVGScript();
        }

         // Physics
        mProjectRuntime->getPhysicsComponent()->setGravity(sceneDefinition->getGravity());

        // Create Root SceneObjectRuntime
        auto sod = sceneDefinition->getRootSceneObjectDefinition();
        auto sor = new SceneObjectRuntime(sod,this);
        if (!sor->useDefinition())
        {
#ifdef DREAM_LOG
            log->error("Error using scene object runtime definition");
#endif
            delete sor;
            sor = nullptr;
            return false;
        }


        setRootSceneObjectRuntime(sor);
        setState(SceneState::SCENE_STATE_LOADED);
#ifdef DREAM_LOG
        mProjectRuntime->getShaderCache()->logShaders();
#endif

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

    ShaderRuntime*
    SceneRuntime::getLightingPassShader
    () const
    {
        return mLightingPassShader;
    }

    void
    SceneRuntime::setLightingPassShader
    (ShaderRuntime* lightingShader)
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
    const
    {
        return mMeshCullDistance;
    }

    void
    SceneRuntime::setMinDrawDistance
    (float f)
    {
        mMinDrawDistance = f;
    }

    float
    SceneRuntime::getMinDrawDistance
    ()
    const
    {
        return mMinDrawDistance;
    }

    float
    SceneRuntime::getMaxDrawDistance
    ()
    const
    {
        return mMaxDrawDistance;
    }

    vector<AssetRuntime*>
    SceneRuntime::getAssetInstances
    (AssetType t)
    const
    {
        vector<AssetRuntime*> instances;
        if (mRootSceneObjectRuntime)
        {
            mRootSceneObjectRuntime->applyToAll
            (
                function<SceneObjectRuntime*(SceneObjectRuntime*)>
                (
                    [&](SceneObjectRuntime* currentRuntime)
                    {
                        AssetRuntime* inst = currentRuntime->getAssetInstance(t);
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
    SceneRuntime::getSceneObjectsWithInstanceOf
    (AssetDefinition* def)
    const
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
                        AssetRuntime* inst = currentRuntime->getAssetInstance(def->getAssetType());
                        if (inst && inst->getUuid() == def->getUuid())
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

    ShaderRuntime*
    SceneRuntime::getShadowPassShader
    ()
    const
    {
        return mShadowPassShader;
    }

    void
    SceneRuntime::setShadowPassShader
    (ShaderRuntime* shadowPassShader)
    {
        mShadowPassShader = shadowPassShader;
    }

    ScriptRuntime*
    SceneRuntime::getInputScript
    ()
    const
    {
        return mInputScript;
    }

    ScriptRuntime*
    SceneRuntime::getNanoVGScript
    ()
    const
    {
        return mNanoVGScript;
    }

    SceneObjectRuntime*
    SceneRuntime::getNearestToCamera
    ()
    {
        if (!mRootSceneObjectRuntime)
        {
            return nullptr;
        }

        float distance = std::numeric_limits<float>::max();
        vec3 camTrans = mCamera.getTranslation();
        SceneObjectRuntime* nearest = mRootSceneObjectRuntime;
        SceneObjectRuntime* focused = mCamera.getFocusedSceneObject();

        mRootSceneObjectRuntime->applyToAll
        (
            function<SceneObjectRuntime*(SceneObjectRuntime*)>
            (
                [&](SceneObjectRuntime* next)
                {
                    if (next == focused)
                    {
                        return nullptr;
                    }
                    float nextDistance = next->distanceFrom(camTrans);
                    if (nextDistance < distance)
                    {
                        distance = nextDistance;
                        nearest = next;
                    }
                   return nullptr;
                }
            )
        );
        return nearest;
    }
}

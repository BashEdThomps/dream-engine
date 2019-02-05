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
#include "SceneObject/SceneObjectTasks.h"

#include "../TaskManager/TaskManager.h"

#include "../Components/Audio/AudioComponent.h"
#include "../Components/Graphics/GraphicsComponent.h"
#include "../Components/Physics/PhysicsComponent.h"
#include "../Components/Script/ScriptComponent.h"

#include "../Components/Graphics/Shader/ShaderCache.h"
#include "../Components/Graphics/Shader/ShaderRuntime.h"
#include "../Components/Graphics/Camera.h"

#include "../Components/Animation/AnimationTasks.h"
#include "../Components/Audio/AudioTasks.h"
#include "../Components/Path/PathTasks.h"
#include "../Components/Scroller/ScrollerTasks.h"

#include "../Components/Time.h"
#include "../Components/Physics/PhysicsObjectRuntime.h"
#include "../Components/Physics/PhysicsTasks.h"
#include "../Components/Input/InputTasks.h"

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

namespace Dream
{
    SceneRuntime::SceneRuntime
    (SceneDefinition* sd, ProjectRuntime* project)
        : Runtime(sd),
          mState(SceneState::SCENE_STATE_TO_LOAD),
          mCamera(Camera(this)),
          mClearColour(vec3(0.0f)),
        mProjectRuntime(project),
        mRootSceneObjectRuntime(nullptr),
        mInputScript(nullptr),
        mNanoVGScript(nullptr),
        mMinDrawDistance(0.1f),
        mMaxDrawDistance(1000.0f),
        mMeshCullDistance(1000.0f),
        mSceneStartTime(0),
        mSceneCurrentTime(0)
    {
        #ifdef DREAM_LOG
        setLogClassName("SceneRuntime");
        getLog()->trace( "Constructing " );
        #endif
    }

    SceneRuntime::~SceneRuntime
    ()
    {
        #ifdef DREAM_LOG
        getLog()->trace("Destructing");
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
        getLog()->debug("Destroying runtime {}",getNameAndUuidString());
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
    const
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
            getLog()->warn("Cannot switch scene state from {} to {}",mState,state);
            #endif
        }
    }

    vec3
    SceneRuntime::getGravity
    ()
    const
    {
        if (mProjectRuntime)
        {
            return mProjectRuntime->getPhysicsComponent()->getGravity();
        }
        return vec3(0.0f);
    }

    void
    SceneRuntime::setGravity
    (const vec3& gravity)
    {
        if (mProjectRuntime)
        {
            mProjectRuntime->getPhysicsComponent()->setGravity(gravity);
        }
    }

    vec3
    SceneRuntime::getClearColour
    ()
    const
    {
        return mClearColour;
    }

    void
    SceneRuntime::setClearColour
    (const vec3& clearColour)
    {
        mClearColour = clearColour;
    }

    SceneObjectRuntime*
    SceneRuntime::getSceneObjectRuntimeByUuid
    (uint32_t uuid)
    const
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
    const
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
    const
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
    const
    {
        if (!mRootSceneObjectRuntime)
        {
            getLog()->debug( "Scenegraph is empty (no root SceneObjectRuntime)" );
            return;
        }

        mRootSceneObjectRuntime->applyToAll
        (
            function<SceneObjectRuntime*(SceneObjectRuntime*)>
            (
                [&](SceneObjectRuntime*)
                {
                    getLog()->debug("showScenegraph not implemented");
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
    const
    {
        return mRootSceneObjectRuntime;
    }

    void
    SceneRuntime::collectGarbage
    ()
    {
        #ifdef DREAM_LOG
        getLog()->debug( "Collecting Garbage {}" , getNameAndUuidString() );
        #endif
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
    const
    {
        return mProjectRuntime;
    }

    bool
    SceneRuntime::hasRootSceneObjectRuntime
    ()
    const
    {
        return mRootSceneObjectRuntime != nullptr;
    }

    bool
    SceneRuntime::useDefinition
    ()
    {
        auto sceneDefinition = dynamic_cast<SceneDefinition*>(mDefinition);

        if (sceneDefinition == nullptr)
        {
            #ifdef DREAM_LOG
            getLog()->error("SceneDefinition is null");
            #endif
            return false;
        }

        #ifdef DREAM_LOG
        getLog()->debug( "Using SceneDefinition ",  sceneDefinition->getNameAndUuidString() );
        #endif

        // Assign Runtime attributes from Definition
        setName(sceneDefinition->getName());
        setUuid(sceneDefinition->getUuid());
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
        mLightingPassShader = dynamic_cast<ShaderRuntime*>(shaderCache->getRuntime(shaderUuid));
        shaderUuid = sceneDefinition->getShadowPassShader();
        mShadowPassShader = dynamic_cast<ShaderRuntime*>(shaderCache->getRuntime(shaderUuid));

        #ifdef DREAM_LOG
        if (mLightingPassShader == nullptr)
        {
            getLog()->error("Unable to load lighting shader {} for Scene {}",shaderUuid,getNameAndUuidString());
        }

        if (mShadowPassShader == nullptr)
        {
            getLog()->error("Unable to load shadow shader {} for Scene {}",shaderUuid,getNameAndUuidString());
        }
        #endif

        // Scripts
        auto scriptCache = mProjectRuntime->getScriptCache();
        auto inputScriptUuid = sceneDefinition->getInputScript();
        mInputScript = dynamic_cast<ScriptRuntime*>(scriptCache->getRuntime(inputScriptUuid));
        if (!mInputScript)
        {
            #ifdef DREAM_LOG
            getLog()->error("Unable to load Input r Script {}",inputScriptUuid);
            #endif
        }
        else
        {
            mInputScript->registerInputScript();
        }

        auto nvgScriptUuid = sceneDefinition->getNanoVGScript();
        mNanoVGScript = dynamic_cast<ScriptRuntime*>(scriptCache->getRuntime(nvgScriptUuid));
        if (!mNanoVGScript)
        {
            #ifdef DREAM_LOG
            getLog()->error("Unable to load NanoVG Script {}",nvgScriptUuid);
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
            getLog()->error("Error using scene object runtime definition");
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
    const
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
    SceneRuntime::getAssetRuntimes
    (AssetType t)
    const
    {
        vector<AssetRuntime*> runtimes;
        if (mRootSceneObjectRuntime)
        {
            mRootSceneObjectRuntime->applyToAll
            (
                function<SceneObjectRuntime*(SceneObjectRuntime*)>
                (
                    [&](SceneObjectRuntime* currentRuntime)
                    {
                        AssetRuntime* inst = currentRuntime->getAssetRuntime(t);
                        if (inst)
                        {
                            runtimes.push_back(inst);
                        }
                        return static_cast<SceneObjectRuntime*>(nullptr);
                    }
                )
            );
        }
        return runtimes;
    }

    vector<SceneObjectRuntime*>
    SceneRuntime::getSceneObjectsWithRuntimeOf
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
                        AssetRuntime* inst = currentRuntime->getAssetRuntime(def->getAssetType());
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
    const
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

    double SceneRuntime::getSceneCurrentTime() const
    {
        return mSceneCurrentTime;
    }

    void SceneRuntime::setSceneCurrentTime(double sceneCurrentTime)
    {
        mSceneCurrentTime = sceneCurrentTime;
    }

    double SceneRuntime::getSceneStartTime() const
    {
        return mSceneStartTime;
    }

    void SceneRuntime::setSceneStartTime(double sceneStartTime)
    {
        mSceneStartTime = sceneStartTime;
    }

    void
    SceneRuntime::createSceneTasks
    ()
    {
        #ifdef DREAM_LOG
        getLog()->debug("Rebuilding SceneObject Update Vector");
        #endif

        auto taskManager = mProjectRuntime->getTaskManager();
        auto physicsComponent = mProjectRuntime->getPhysicsComponent();
        auto graphicsComponent = mProjectRuntime->getGraphicsComponent();
        auto inputComponent = mProjectRuntime->getInputComponent();

        Task* pollDataTask = nullptr;
        Task* executeInputTask = nullptr;

        if (!inputComponent->hasPollDataTask())
        {
            pollDataTask = new InputPollDataTask(inputComponent);

            if (!inputComponent->hasExecuteScriptTask())
            {
                executeInputTask = new InputExecuteScriptTask(inputComponent,this);
                executeInputTask->dependsOn(pollDataTask);
            }

            taskManager->pushTask(pollDataTask);
            taskManager->pushTask(executeInputTask);

        }
        vector<Task*> lifetimeTasks;

        mRootSceneObjectRuntime->applyToAll
        (
            function<SceneObjectRuntime*(SceneObjectRuntime*)>(
            [&](SceneObjectRuntime* rt)
            {
                Task* lt = nullptr;
                rt->lock();
                // SceneObject
                if (!rt->hasLifetimeUpdateTask())
                {
                    lt = (new LifetimeUpdateTask(rt))->dependsOn(executeInputTask);
                    lifetimeTasks.push_back(lt);

                    // Animation
                    if (rt->hasAnimationRuntime())
                    {
                        auto anim = rt->getAnimationRuntime();
                        if (!anim->hasUpdateTask())
                        {
                            taskManager->pushTask((new AnimationUpdateTask(anim))->dependsOn(lt));
                        }
                    }
                    // Audio
                    if (rt->hasAudioRuntime())
                    {
                        auto audio = rt->getAudioRuntime();
                        if (!audio->hasMarkersUpdateTask())
                        {
                            taskManager->pushTask((new AudioMarkersUpdateTask(audio))->dependsOn(lt));
                        }
                    }
                    // Graphics
                    if (!rt->getHidden() && rt->hasLightRuntime())
                    {
                       graphicsComponent->addToLightQueue(rt);
                    }
                    // Physics
                    if (rt->hasPhysicsObjectRuntime())
                    {
                        auto pObj = rt->getPhysicsObjectRuntime();
                        if (!pObj->isInPhysicsWorld() && !pObj->hasUpdateTask())
                        {
                            taskManager->pushTask((new PhysicsAddObjectTask(physicsComponent, pObj))->dependsOn(lt));
                        }
                    }
                    // Path
                    if (rt->hasPathRuntime())
                    {
                        auto path = rt->getPathRuntime();
                        if (!path->hasUpdateTask())
                        {
                            taskManager->pushTask((new PathUpdateTask(path))->dependsOn(lt));
                        }
                    }
                    // Scroller
                    if (rt->hasScrollerRuntime())
                    {
                        auto scr = rt->getScrollerRuntime();
                        if (!scr->hasUpdateTask())
                        {
                            taskManager->pushTask((new ScrollerUpdateTask(scr))->dependsOn(lt));
                        }
                    }
                    taskManager->pushTask(lt);
                }
                rt->unlock();
                return static_cast<SceneObjectRuntime*>(nullptr);
            }
        ));

        if (!physicsComponent->hasUpdateWorldTask())
        {
            auto pt = new PhysicsUpdateWorldTask(physicsComponent);
            for (auto* lt : lifetimeTasks)
            {
                pt->dependsOn(lt);
            }
            taskManager->pushTask(pt);
        }

        if (physicsComponent->getDebug())
        {
            graphicsComponent->pushTask(new PhysicsDrawDebugTask(physicsComponent));
        }

        taskManager->clearFences();
    }


    void
    SceneRuntime::updateLifetime
    ()
    {
        auto time = mProjectRuntime->getTime();
        long timeDelta = time->getFrameTimeDelta();
        if (timeDelta <= Time::DELTA_MAX)
        {
            long frameTime = time->getCurrentFrameTime();
            if (getSceneStartTime() <= 0)
            {
               setSceneStartTime(frameTime);
            }
            setSceneCurrentTime(frameTime-getSceneStartTime());
        }
    }
}

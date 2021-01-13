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

#include "SceneDefinition.h"
#include "Entity/EntityDefinition.h"
#include "Entity/EntityRuntime.h"
#include "Common/Logger.h"
#include "Components/Audio/AudioComponent.h"
#include "Components/Graphics/GraphicsComponent.h"
#include "Components/Physics/PhysicsComponent.h"
#include "Components/Script/ScriptComponent.h"
#include "Components/Input/InputComponent.h"
#include "Components/Animation/AnimationTasks.h"
#include "Components/Audio/AudioTasks.h"
#include "Components/Path/PathTasks.h"
#include "Components/Scroller/ScrollerTasks.h"
#include "Components/Physics/PhysicsTasks.h"
#include "Components/Input/InputTasks.h"
#include "Components/Time.h"
#include "Components/Graphics/Camera.h"
#include "Components/Animation/AnimationRuntime.h"
#include "Components/Graphics/Shader/ShaderRuntime.h"
#include "Components/Audio/AudioRuntime.h"
#include "Components/Physics/PhysicsObjectRuntime.h"
#include "Components/Path/PathRuntime.h"
#include "Components/Scroller/ScrollerRuntime.h"
#include "Components/Graphics/Shader/ShaderCache.h"
#include "Components/ObjectEmitter/ObjectEmitterRuntime.h"
#include "Components/ObjectEmitter/ObjectEmitterTasks.h"
#include "Components/Script/ScriptRuntime.h"
#include "Project/ProjectRuntime.h"
#include "TaskManager/TaskManager.h"
#include <iostream>

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
        mClearColour(Vector3(0.0f)),
        mProjectRuntime(project),
        mRootEntityRuntime(nullptr),
        mLightingPassShader(nullptr),
        mShadowPassShader(nullptr),
        mInputScript(nullptr),
        mCamera(Camera(this)),
        mSceneStartTime(0),
        mSceneCurrentTime(0),
        mMinDrawDistance(0.1f),
        mMaxDrawDistance(1000.0f),
        mMeshCullDistance(1000.0f),
        mPlayerObject(nullptr)
    {
        LOG_TRACE( "Constructing " );
    }

    SceneRuntime::~SceneRuntime
    ()
    {
        LOG_TRACE("Destructing");
        if (mState != SCENE_STATE_DESTROYED)
        {
            destroyRuntime();
        }
    }

    void
    SceneRuntime::destroyRuntime
    ()
    {
        LOG_DEBUG("Destroying runtime {}",getNameAndUuidString());

        if (mRootEntityRuntime != nullptr)
        {
            delete mRootEntityRuntime;
            mRootEntityRuntime = nullptr;
        }

        mLightingPassShader = nullptr;
        mEntityRuntimeCleanUpQueue.clear();
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
            LOG_WARN("Cannot switch scene state from {} to {}",mState,state);
        }
    }

    Vector3
    SceneRuntime::getGravity
    ()
    const
    {
        if (mProjectRuntime)
        {
            return mProjectRuntime->getPhysicsComponent()->getGravity();
        }
        return Vector3(0.0f);
    }

    void
    SceneRuntime::setGravity
    (const Vector3& gravity)
    {
        if (mProjectRuntime)
        {
            mProjectRuntime->getPhysicsComponent()->setGravity(gravity);
        }
    }

    Vector3
    SceneRuntime::getClearColour
    ()
    const
    {
        return mClearColour;
    }

    void
    SceneRuntime::setClearColour
    (const Vector3& clearColour)
    {
        mClearColour = clearColour;
    }

    EntityRuntime*
    SceneRuntime::getEntityRuntimeByUuid
    (uint32_t uuid)
    const
    {
        if (!mRootEntityRuntime)
        {
            return nullptr;
        }

        return mRootEntityRuntime->applyToAll
        (
            function<EntityRuntime*(EntityRuntime*)>
            (
                [&](EntityRuntime* currentRuntime)
                {
                    if (!currentRuntime)
                    {
                        return static_cast<EntityRuntime*>(nullptr);
                    }
                    if (currentRuntime->hasUuid(uuid))
                    {
                        return currentRuntime;
                    }
                    return static_cast<EntityRuntime*>(nullptr);
                }
            )
        );
    }

    EntityRuntime*
    SceneRuntime::getEntityRuntimeByName
    (const string& name)
    const
    {
        if (!mRootEntityRuntime)
        {
            return nullptr;
        }
        return mRootEntityRuntime->applyToAll
        (
            function<EntityRuntime*(EntityRuntime*)>
            (
                [&](EntityRuntime* currentRuntime)
                {
                    if (!currentRuntime)
                    {
                        return static_cast<EntityRuntime*>(nullptr);
                    }

                    if (currentRuntime->hasName(name))
                    {
                        return currentRuntime;
                    }
                    return static_cast<EntityRuntime*>(nullptr);
                }
            )
        );
    }

    int
    SceneRuntime::countEntityRuntimes
    ()
    const
    {
        if (!mRootEntityRuntime)
        {
            return 0;
        }
        int count = 0;
        mRootEntityRuntime->applyToAll
        (
            function<EntityRuntime*(EntityRuntime*)>
            (
                [&](EntityRuntime*)
                {
                    count++;
                    return static_cast<EntityRuntime*>(nullptr);
                }
            )
        );
        return count;
    }

    void
    SceneRuntime::showScenegraph
    ()
    const
    {
        if (!mRootEntityRuntime)
        {
            LOG_DEBUG( "Scenegraph is empty (no root EntityRuntime)" );
            return;
        }

        mRootEntityRuntime->applyToAll
        (
            function<EntityRuntime*(EntityRuntime*)>
            (
                [&](EntityRuntime*)
                {
                    LOG_DEBUG("showScenegraph not implemented");
                    //obj->showStatus();
                    return nullptr;
                }
            )
        );
    }

    void
    SceneRuntime::setRootEntityRuntime
    (EntityRuntime* root)
    {
        mRootEntityRuntime = root;
    }

    EntityRuntime*
    SceneRuntime::getRootEntityRuntime
    ()
    const
    {
        return mRootEntityRuntime;
    }

    void
    SceneRuntime::collectGarbage
    ()
    {
        LOG_DEBUG( "Collecting Garbage {}" , getNameAndUuidString() );
        mRootEntityRuntime->applyToAll
        (
            function<EntityRuntime*(EntityRuntime*)>
            (
                [&](EntityRuntime* runt)
                {
                    runt->collectGarbage();
                    return static_cast<EntityRuntime*>(nullptr);
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
    SceneRuntime::hasRootEntityRuntime
    ()
    const
    {
        return mRootEntityRuntime != nullptr;
    }

    bool
    SceneRuntime::useDefinition
    ()
    {
        auto sceneDefinition = dynamic_cast<SceneDefinition*>(mDefinition);

        if (sceneDefinition == nullptr)
        {
            LOG_ERROR("SceneDefinition is null");
            return false;
        }

        LOG_DEBUG( "Using SceneDefinition ",  sceneDefinition->getNameAndUuidString() );

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

        if (mLightingPassShader == nullptr)
        {
            LOG_ERROR("Unable to load lighting shader {} for Scene {}",shaderUuid,getNameAndUuidString());
        }

        if (mShadowPassShader == nullptr)
        {
            LOG_ERROR("Unable to load shadow shader {} for Scene {}",shaderUuid,getNameAndUuidString());
        }

        // Scripts
        auto scriptCache = mProjectRuntime->getScriptCache();
        auto inputScriptUuid = sceneDefinition->getInputScript();
        mInputScript = dynamic_cast<ScriptRuntime*>(scriptCache->getRuntime(inputScriptUuid));
        if (!mInputScript)
        {
            LOG_ERROR("Unable to load Input r Script {}",inputScriptUuid);
        }

         // Physics
         mProjectRuntime->getPhysicsComponent()->setGravity(sceneDefinition->getGravity());

        // Create Root EntityRuntime
        auto sod = sceneDefinition->getRootEntityDefinition();
        auto sor = new EntityRuntime(sod,this);
        if (!sor->useDefinition())
        {
            LOG_ERROR("Error using scene object runtime definition");
            delete sor;
            sor = nullptr;
            return false;
        }


        setRootEntityRuntime(sor);
        setState(SceneState::SCENE_STATE_LOADED);
        mProjectRuntime->getShaderCache()->logShaders();

        auto focused = getEntityRuntimeByUuid(sceneDefinition->getCameraFocusedOn());
        mCamera.setFocusedSceneObejct(focused);

        auto player = getEntityRuntimeByUuid(sceneDefinition->getPlayerObject());
        setPlayerObject(player);

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
        if (mRootEntityRuntime)
        {
            mRootEntityRuntime->applyToAll
            (
                function<EntityRuntime*(EntityRuntime*)>
                (
                    [&](EntityRuntime* currentRuntime)
                    {
                        AssetRuntime* inst = currentRuntime->getAssetRuntime(t);
                        if (inst)
                        {
                            runtimes.push_back(inst);
                        }
                        return static_cast<EntityRuntime*>(nullptr);
                    }
                )
            );
        }
        return runtimes;
    }

    vector<EntityRuntime*>
    SceneRuntime::getEntitysWithRuntimeOf
    (AssetDefinition* def)
    const
    {
        vector<EntityRuntime*> runtimes;
        if (mRootEntityRuntime)
        {
            mRootEntityRuntime->applyToAll
            (
                function<EntityRuntime*(EntityRuntime*)>
                (
                    [&](EntityRuntime* currentRuntime)
                    {
                        AssetRuntime* inst = currentRuntime->getAssetRuntime(def->getAssetType());
                        if (inst && inst->getUuid() == def->getUuid())
                        {
                            runtimes.push_back(currentRuntime);
                        }
                        return static_cast<EntityRuntime*>(nullptr);
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

    EntityRuntime*
    SceneRuntime::getNearestToCamera
    ()
    const
    {
        if (!mRootEntityRuntime)
        {
            return nullptr;
        }

        float distance = std::numeric_limits<float>::max();
        Vector3 camTrans = mCamera.getTranslation();
        EntityRuntime* nearest = mRootEntityRuntime;
        EntityRuntime* focused = mCamera.getFocusedEntity();

        mRootEntityRuntime->applyToAll
        (
            function<EntityRuntime*(EntityRuntime*)>
            (
                [&](EntityRuntime* next)
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

    unsigned long
    SceneRuntime::getSceneCurrentTime
    () const
    {
        return mSceneCurrentTime;
    }

    void SceneRuntime::setSceneCurrentTime(unsigned long sceneCurrentTime)
    {
        mSceneCurrentTime = sceneCurrentTime;
    }

    unsigned long
    SceneRuntime::getSceneStartTime
    () const
    {
        return mSceneStartTime;
    }

    void
    SceneRuntime::setSceneStartTime
    (unsigned long sceneStartTime)
    {
        mSceneStartTime = sceneStartTime;
    }

    void
    SceneRuntime::createSceneTasks
    ()
    {

        LOG_DEBUG("Building SceneRuntime Task Queue...");

        updateLifetime();

        auto taskManager = mProjectRuntime->getTaskManager();
        auto physicsComponent = mProjectRuntime->getPhysicsComponent();
        auto graphicsComponent = mProjectRuntime->getGraphicsComponent();
        auto inputComponent = mProjectRuntime->getInputComponent();
        auto scriptComponent = mProjectRuntime->getScriptComponent();

        auto constructInput = mInputScript->getConstructionTask();
        if (constructInput->getState() == TaskState::NEW)
        {
           constructInput->setState(TaskState::QUEUED);
           taskManager->pushTask(constructInput);
        }
        else if (constructInput->getState() == TaskState::COMPLETED)
        {
            // Poll Data
            inputComponent->setCurrentSceneRuntime(this);
            auto poll = inputComponent->getPollDataTask();
            poll->clearState();
            taskManager->pushTask(poll);

            // Process Input
            auto exec =inputComponent->getExecuteScriptTask();
            exec->clearState();
            exec->dependsOn(inputComponent->getPollDataTask());
            taskManager->pushTask(exec);
        }

        PhysicsUpdateWorldTask* physicsUpdate = nullptr;
        if (physicsComponent->getEnabled())
        {
            physicsUpdate = physicsComponent->getUpdateWorldTask();
            physicsUpdate->clearState();
            taskManager->pushTask(physicsUpdate);
        }

        // Process Entitys
        mRootEntityRuntime->applyToAll
        (
            function<EntityRuntime*(EntityRuntime*)>(
            [&](EntityRuntime* rt)
            {
                rt->updateLifetime();
                // Animation
                if (rt->hasAnimationRuntime())
                {
                    auto anim = rt->getAnimationRuntime();
                    auto ut = anim->getUpdateTask();
                    ut->clearState();
                    taskManager->pushTask(ut);
                }
                // Audio
                if (rt->hasAudioRuntime())
                {
                    auto audio = rt->getAudioRuntime();
                    auto ut = audio->getMarkersUpdateTask();
                    ut->clearState();
                    taskManager->pushTask(ut);
                }

                // Physics
                if (physicsComponent->getEnabled() && rt->hasPhysicsObjectRuntime())
                {
                    auto pObj = rt->getPhysicsObjectRuntime();
                    if (!pObj->isInPhysicsWorld())
                    {
                        auto ut = pObj->getAddObjectTask();
                        ut->clearState();
                        ut->dependsOn(physicsUpdate);
                        taskManager->pushTask(ut);
                    }
                }
                // Path
                if (rt->hasPathRuntime())
                {
                    auto path = rt->getPathRuntime();
                    auto ut = path->getUpdateTask();
                    ut->clearState();
                    taskManager->pushTask(ut);
                }
                // Scroller
                if (rt->hasScrollerRuntime())
                {
                    auto scr = rt->getScrollerRuntime();
                    auto ut = scr->getUpdateTask();
                    ut->clearState();
                    taskManager->pushTask(ut);
                }

                // Object Emitter
                if (rt->hasObjectEmitterRuntime())
                {
                    auto oe = rt->getObjectEmitterRuntime();
                    auto ut = oe->getUpdateTask();
                    ut->clearState();
                    taskManager->pushTask(ut);
                }

                // Scripting
                if (scriptComponent->getEnabled() && rt->hasScriptRuntime())
                {
                    auto script = rt->getScriptRuntime();
                    auto load = script->getConstructionTask();
                    if (load->getState() == TaskState::NEW)
                    {
                        // Don't clear state of load
                        load->setState(TaskState::QUEUED);
                        taskManager->pushTask(load);
                    }
                    else if (load->getState() == TaskState::COMPLETED)
                    {
                        if (!script->getInitialised(rt))
                        {
                            auto init = rt->getScriptOnInitTask();
                            init->clearState();
                            taskManager->pushTask(init);
                        }
                        else
                        {
                            if (rt->hasEvents())
                            {
                                auto event = rt->getScriptOnEventTask();
                                event->clearState();
                                event->dependsOn(physicsUpdate);
                                taskManager->pushTask(event);
                            }

                            auto update = rt->getScriptOnUpdateTask();
                            update->clearState();
                            taskManager->pushTask(update);
                        }
                    }
                }

                // Graphics
                if (!rt->getHidden() && rt->hasLightRuntime())
                {
                   graphicsComponent->addToLightQueue(rt);
                }
                //rt->unlock();
                return static_cast<EntityRuntime*>(nullptr);
            }
        ));

        if (physicsComponent->getDebug())
        {
            auto drawDebug = physicsComponent->getDrawDebugTask();
            drawDebug->clearState();
            graphicsComponent->pushTask(drawDebug);
        }
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

    void
    SceneRuntime::setPlayerObject
    (EntityRuntime* po)
    {
        mPlayerObject = po;
    }

    EntityRuntime*
    SceneRuntime::getPlayerObject
    () const
    {
       return mPlayerObject;
    }
}

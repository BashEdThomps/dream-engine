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
#include "Components/Physics/PhysicsTasks.h"
#include "Components/Input/InputTasks.h"
#include "Components/Time.h"
#include "Components/Graphics/Camera.h"
#include "Components/Animation/AnimationRuntime.h"
#include "Components/Graphics/Shader/ShaderRuntime.h"
#include "Components/Audio/AudioRuntime.h"
#include "Components/Physics/PhysicsObjectRuntime.h"
#include "Components/Path/PathRuntime.h"
#include "Components/Graphics/Shader/ShaderCache.h"
#include "Components/Script/ScriptRuntime.h"
#include "Project/ProjectRuntime.h"
#include "Components/Task/TaskManager.h"
#include <iostream>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

using std::make_shared;

namespace octronic::dream
{
    SceneRuntime::SceneRuntime
    (SceneDefinition* sd, ProjectRuntime* project)
        : Runtime("SceneRuntime",sd),
          mState(SceneState::SCENE_STATE_TO_LOAD),
          mClearColour(Vector3(0.0f)),
          mProjectRuntime(project),
          mRootEntityRuntime(nullptr),
          mLightingPassShader(nullptr),
          mShadowPassShader(nullptr),
          mFontShader(nullptr),
          mSpriteShader(nullptr),
          mInputScript(nullptr),
          mCamera(Camera(this)),
          mSceneStartTime(0),
          mSceneCurrentTime(0),
          mMinDrawDistance(0.1f),
          mMaxDrawDistance(1000.0f),
          mMeshCullDistance(1000.0f),
          mPlayerEntity(nullptr),
          mInputScriptConstructionTask(),
          mInputScriptDestructionTask(nullptr)
    {
        LOG_TRACE( "SceneRuntime: Constructing " );
    }

    SceneRuntime::~SceneRuntime
    ()
    {
        LOG_TRACE("SceneRuntime: Destructing");
        if (mState != SCENE_STATE_DESTROYED)
        {
            destroyRuntime();
        }
    }

    void
    SceneRuntime::destroyRuntime
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            LOG_DEBUG("SceneRuntime: Destroying runtime {}",getNameAndUuidString());

            if (mRootEntityRuntime != nullptr)
            {
                delete mRootEntityRuntime;
                mRootEntityRuntime = nullptr;
            }

            mLightingPassShader = nullptr;
            mShadowPassShader = nullptr;
            mFontShader = nullptr;
            mEntityRuntimeCleanUpQueue.clear();
            mState = SceneState::SCENE_STATE_DESTROYED;
        } dreamElseLockFailed

    }

    SceneState
    SceneRuntime::getState
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return mState;
        } dreamElseLockFailed
    }

    void
    SceneRuntime::setState
    (SceneState state)
    {
        if(dreamTryLock())
        {
            dreamLock();
            if (state >= mState || (mState == SCENE_STATE_ACTIVE && state == SCENE_STATE_LOADED))
            {
                mState = state;
            }
            else
            {
                LOG_WARN("SceneRuntime: Cannot switch scene state from {} to {}",mState,state);
            }
        } dreamElseLockFailed
    }

    Vector3
    SceneRuntime::getGravity
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            if (mProjectRuntime)
            {
                return mProjectRuntime->getPhysicsComponent()->getGravity();
            }
            return Vector3(0.0f);
        } dreamElseLockFailed
    }

    void
    SceneRuntime::setGravity
    (const Vector3& gravity)
    {
        if(dreamTryLock())
        {
            dreamLock();
            if (mProjectRuntime)
            {
                mProjectRuntime->getPhysicsComponent()->setGravity(gravity);
            }
        } dreamElseLockFailed
    }

    Vector3
    SceneRuntime::getClearColour
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return mClearColour;
        } dreamElseLockFailed
    }

    void
    SceneRuntime::setClearColour
    (const Vector3& clearColour)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mClearColour = clearColour;
        } dreamElseLockFailed
    }

    EntityRuntime*
    SceneRuntime::getEntityRuntimeByUuid
    (UuidType uuid)
    {
        if(dreamTryLock())
        {
            dreamLock();
            if (!mRootEntityRuntime)
            {
                return nullptr;
            }

            return mRootEntityRuntime->applyToAll(
                        function<EntityRuntime*(EntityRuntime*)>
                        ([&](EntityRuntime* currentRuntime){
                            if (!currentRuntime)
                            {
                                return static_cast<EntityRuntime*>(nullptr);
                            }
                            if (currentRuntime->hasUuid(uuid))
                            {
                                return currentRuntime;
                            }
                            return static_cast<EntityRuntime*>(nullptr);
                        }));
        } dreamElseLockFailed
    }

    EntityRuntime*
    SceneRuntime::getEntityRuntimeByName
    (const string& name)
    {
        if(dreamTryLock())
        {
            dreamLock();

            if (!mRootEntityRuntime)
            {
                return nullptr;
            }

            return mRootEntityRuntime->applyToAll(
                        function<EntityRuntime*(EntityRuntime*)>
                        ([&](EntityRuntime* currentRuntime){
                            if (!currentRuntime)
                            {
                                return static_cast<EntityRuntime*>(nullptr);
                            }

                            if (currentRuntime->hasName(name))
                            {
                                return currentRuntime;
                            }
                            return static_cast<EntityRuntime*>(nullptr);
                        }));
        } dreamElseLockFailed
    }

    int
    SceneRuntime::countEntityRuntimes
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();

            if (!mRootEntityRuntime)
            {
                return 0;
            }
            int count = 0;
            mRootEntityRuntime->applyToAll(
                        function<EntityRuntime*(EntityRuntime*)>
                        ([&](EntityRuntime*){
                            count++;
                            return static_cast<EntityRuntime*>(nullptr);
                        }));
            return count;
        } dreamElseLockFailed
    }

    void
    SceneRuntime::showScenegraph
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();

            if (!mRootEntityRuntime)
            {
                LOG_DEBUG( "SceneRuntime: Scenegraph is empty (no root EntityRuntime)" );
                return;
            }

            mRootEntityRuntime->applyToAll(
                        function<EntityRuntime*(EntityRuntime*)>
                        ([&](EntityRuntime*){
                            LOG_DEBUG("SceneRuntime: showScenegraph not implemented");
                            //obj->showStatus();
                            return nullptr;
                        }));
        } dreamElseLockFailed
    }

    void
    SceneRuntime::setRootEntityRuntime
    (EntityRuntime* root)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mRootEntityRuntime = root;
        } dreamElseLockFailed
    }

    EntityRuntime*
    SceneRuntime::getRootEntityRuntime
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return mRootEntityRuntime;
        } dreamElseLockFailed
    }

    void
    SceneRuntime::collectGarbage
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            LOG_DEBUG( "SceneRuntime: Collecting Garbage {}" , getNameAndUuidString() );
            mRootEntityRuntime->applyToAll(
                        function<EntityRuntime*(EntityRuntime*)>
                        ([&](EntityRuntime* runt){
                            runt->collectGarbage();
                            return static_cast<EntityRuntime*>(nullptr);
                        }));
        } dreamElseLockFailed
    }

    ProjectRuntime*
    SceneRuntime::getProjectRuntime
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return mProjectRuntime;
        } dreamElseLockFailed
    }

    bool
    SceneRuntime::hasRootEntityRuntime
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return mRootEntityRuntime != nullptr;
        } dreamElseLockFailed
    }

    bool
    SceneRuntime::useDefinition
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            SceneDefinition* sceneDefinition = dynamic_cast<SceneDefinition*>(mDefinition);

            if (sceneDefinition == nullptr)
            {
                LOG_ERROR("SceneRuntime: SceneDefinition is null");
                return false;
            }

            LOG_DEBUG( "SceneRuntime: Using SceneDefinition ",  sceneDefinition->getNameAndUuidString() );

            // Assign Runtime attributes from Definition
            setName(sceneDefinition->getName());
            setUuid(sceneDefinition->getUuid());
            setClearColour(sceneDefinition->getClearColour());

            // Setup Camera
            mCamera.setTranslation(sceneDefinition->getCameraTranslation());
            mCamera.setMovementSpeed(sceneDefinition->getCameraMovementSpeed());
            mCamera.setPitch(sceneDefinition->getCameraPitch());
            mCamera.setYaw(sceneDefinition->getCameraYaw());

            //  Setup drawing parameters
            setMeshCullDistance(sceneDefinition->getMeshCullDistance());
            setMinDrawDistance(sceneDefinition->getMinDrawDistance());
            setMaxDrawDistance(sceneDefinition->getMaxDrawDistance());

            // Load Scene-level Shaders
            ShaderCache* shaderCache = mProjectRuntime->getShaderCache();

            UuidType lightPassShaderUuid = sceneDefinition->getLightingPassShader();
            mLightingPassShader = dynamic_cast<ShaderRuntime*>(shaderCache->getRuntime(lightPassShaderUuid));

            if (mLightingPassShader == nullptr)
            {
                LOG_ERROR("SceneRuntime: Unable to load lighting shader {} for Scene {}",lightPassShaderUuid,getNameAndUuidString());
            }

            UuidType shadowPassShaderUuid = sceneDefinition->getShadowPassShader();
            mShadowPassShader = dynamic_cast<ShaderRuntime*>(shaderCache->getRuntime(shadowPassShaderUuid));

            if (mShadowPassShader == nullptr)
            {
                LOG_ERROR("SceneRuntime: Unable to load shadow shader {} for Scene {}",shadowPassShaderUuid,getNameAndUuidString());
            }

            UuidType fontShaderUuid = sceneDefinition->getFontShader();
            mFontShader = dynamic_cast<ShaderRuntime*>(shaderCache->getRuntime(fontShaderUuid));

            if (mFontShader == nullptr)
            {
                LOG_ERROR("SceneRuntime: Unable to load font shader {} for Scene {}",fontShaderUuid,getNameAndUuidString());
            }

            UuidType spriteShaderUuid = sceneDefinition->getSpriteShader();
            mSpriteShader = dynamic_cast<ShaderRuntime*>(shaderCache->getRuntime(spriteShaderUuid));

            if (mSpriteShader == nullptr)
            {
                LOG_ERROR("SceneRuntime: Unable to load sprite shader {} for Scene {}",spriteShaderUuid,getNameAndUuidString());
            }

            mProjectRuntime->getShaderCache()->logShaders();

            // Scripts
            ScriptCache* scriptCache = mProjectRuntime->getScriptCache();
            UuidType inputScriptUuid = sceneDefinition->getInputScript();
            mInputScript = dynamic_cast<ScriptRuntime*>(scriptCache->getRuntime(inputScriptUuid));
            if (mInputScript)
            {
                LOG_TRACE("SceneRuntime: Setting up input script Tasks");
                mInputScriptConstructionTask.setScript(mInputScript);
                mInputScriptDestructionTask = make_shared<InputScriptDestructionTask>();
                mInputScriptDestructionTask->setScript(mInputScript);
            }
            else
            {
                LOG_ERROR("SceneRuntime: Unable to load Input r Script {}",inputScriptUuid);
            }

            // Physics
            mProjectRuntime->getPhysicsComponent()->setGravity(sceneDefinition->getGravity());

            // Create Root EntityRuntime
            EntityDefinition* entityDef = sceneDefinition->getRootEntityDefinition();
            EntityRuntime* er = new EntityRuntime(entityDef,this);
            if (!er->useDefinition())
            {
                LOG_ERROR("SceneRuntime: Error using scene object runtime definition");
                delete er;
                er = nullptr;
                return false;
            }

            setRootEntityRuntime(er);
            setState(SceneState::SCENE_STATE_LOADED);

            EntityRuntime* focused = getEntityRuntimeByUuid(sceneDefinition->getCameraFocusedOn());
            mCamera.setFocusedEntity(focused);

            EntityRuntime* player = getEntityRuntimeByUuid(sceneDefinition->getPlayerObject());
            setPlayerEntity(player);

            return true;
        } dreamElseLockFailed
    }

    bool
    SceneRuntime::getPhysicsDebug
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            if (mProjectRuntime)
            {
                return mProjectRuntime->getPhysicsComponent()->getDebug();
            }
            return false;
        } dreamElseLockFailed
    }

    void
    SceneRuntime::setPhysicsDebug
    (bool physicsDebug)
    {
        if(dreamTryLock())
        {
            dreamLock();
            if (mProjectRuntime)
            {
                mProjectRuntime->getPhysicsComponent()->setDebug(physicsDebug);
            }
        } dreamElseLockFailed
    }

    ShaderRuntime*
    SceneRuntime::getLightingPassShader
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return mLightingPassShader;
        } dreamElseLockFailed
    }

    void
    SceneRuntime::setLightingPassShader
    (ShaderRuntime* lightingShader)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mLightingPassShader = lightingShader;
        } dreamElseLockFailed
    }

    void
    SceneRuntime::setMeshCullDistance
    (float mcd)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mMeshCullDistance = mcd;
        } dreamElseLockFailed
    }

    float
    SceneRuntime::getMeshCullDistance
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return mMeshCullDistance;
        } dreamElseLockFailed
    }

    void
    SceneRuntime::setMinDrawDistance
    (float f)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mMinDrawDistance = f;
        } dreamElseLockFailed
    }

    float
    SceneRuntime::getMinDrawDistance
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return mMinDrawDistance;
        } dreamElseLockFailed
    }

    float
    SceneRuntime::getMaxDrawDistance
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return mMaxDrawDistance;
        } dreamElseLockFailed
    }

    vector<AssetRuntime*>
    SceneRuntime::getAssetRuntimes
    (AssetType t)
    {
        if(dreamTryLock())
        {
            dreamLock();
            vector<AssetRuntime*> runtimes;
            if (mRootEntityRuntime)
            {
                mRootEntityRuntime->applyToAll(
                            function<EntityRuntime*(EntityRuntime*)>
                            ([&](EntityRuntime* currentRuntime){
                                AssetRuntime* inst = currentRuntime->getAssetRuntime(t);
                                if (inst)
                                {
                                    runtimes.push_back(inst);
                                }
                                return static_cast<EntityRuntime*>(nullptr);
                            }));
            }
            return runtimes;
        } dreamElseLockFailed
    }

    vector<EntityRuntime*>
    SceneRuntime::getEntitysWithRuntimeOf
    (AssetDefinition* def)
    {
        if(dreamTryLock())
        {
            dreamLock();

            vector<EntityRuntime*> runtimes;
            if (mRootEntityRuntime)
            {
                mRootEntityRuntime->applyToAll(
                            function<EntityRuntime*(EntityRuntime*)>
                            ([&](EntityRuntime* currentRuntime){
                                AssetRuntime* inst = currentRuntime->getAssetRuntime(def->getAssetType());
                                if (inst && inst->getUuid() == def->getUuid())
                                {
                                    runtimes.push_back(currentRuntime);
                                }
                                return static_cast<EntityRuntime*>(nullptr);
                            }));
            }
            return runtimes;
        } dreamElseLockFailed
    }


    void
    SceneRuntime::setMaxDrawDistance
    (float f)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mMaxDrawDistance = f;
        } dreamElseLockFailed
    }

    Camera*
    SceneRuntime::getCamera
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return &mCamera;
        } dreamElseLockFailed
    }

    ShaderRuntime*
    SceneRuntime::getShadowPassShader
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return mShadowPassShader;
        } dreamElseLockFailed
    }

    void
    SceneRuntime::setShadowPassShader
    (ShaderRuntime* shadowPassShader)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mShadowPassShader = shadowPassShader;
        } dreamElseLockFailed
    }

    ShaderRuntime* SceneRuntime::getFontShader()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return mFontShader;
        } dreamElseLockFailed
    }

    void SceneRuntime::setFontShader(ShaderRuntime* fontShader)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mFontShader = fontShader;
        } dreamElseLockFailed
    }

    ShaderRuntime* SceneRuntime::getSpriteShader()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return mSpriteShader;
        } dreamElseLockFailed
    }

    void SceneRuntime::setSpriteShader(ShaderRuntime* shader)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mSpriteShader = shader;
        } dreamElseLockFailed
    }

    ScriptRuntime*
    SceneRuntime::getInputScript
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return mInputScript;
        } dreamElseLockFailed
    }

    EntityRuntime*
    SceneRuntime::getNearestToCamera
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            if (!mRootEntityRuntime)
            {
                return nullptr;
            }

            float distance = std::numeric_limits<float>::max();
            Vector3 camTrans = mCamera.getTranslation();
            EntityRuntime* nearest = mRootEntityRuntime;
            EntityRuntime* focused = mCamera.getFocusedEntity();

            mRootEntityRuntime->applyToAll(
                        function<EntityRuntime*(EntityRuntime*)>
                        ([&](EntityRuntime* next){
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
                        }));
            return nearest;
        } dreamElseLockFailed
    }

    unsigned long
    SceneRuntime::getSceneCurrentTime
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return mSceneCurrentTime;
        } dreamElseLockFailed
    }

    void SceneRuntime::setSceneCurrentTime(unsigned long sceneCurrentTime)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mSceneCurrentTime = sceneCurrentTime;
        } dreamElseLockFailed
    }

    unsigned long
    SceneRuntime::getSceneStartTime
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return mSceneStartTime;
        } dreamElseLockFailed
    }

    void
    SceneRuntime::setSceneStartTime
    (unsigned long sceneStartTime)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mSceneStartTime = sceneStartTime;
        } dreamElseLockFailed
    }

    /**
     * @brief SceneRuntime::createSceneTasks
     *
     * Entitys with DiscreetAssetRuntimes need to push back their own tasks,
     * All SharedAssetRuntimes should be pushed back only once.
     * This is handled by TaskThread->hasTask, to check if any of the threads
     * already have a copy of the task.
     */
    void
    SceneRuntime::createSceneTasks
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();

            LOG_DEBUG("SceneRuntime: Building SceneRuntime Task Queue...");

            updateLifetime();

            // Get Components
            TaskManager* taskManager = mProjectRuntime->getTaskManager();
            PhysicsComponent* physicsComponent = mProjectRuntime->getPhysicsComponent();
            GraphicsComponent* graphicsComponent = mProjectRuntime->getGraphicsComponent();
            InputComponent* inputComponent = mProjectRuntime->getInputComponent();
            ScriptComponent* scriptComponent = mProjectRuntime->getScriptComponent();

            // Input component needs to be constructed
            if (mInputScript && mInputScriptConstructionTask.getState() == TaskState::TASK_STATE_CONSTRUCTED)
            {
                //taskManager->pushTask(&mInputScriptConstructionTask);
                mInputScriptConstructionTask.execute();
            }
            // Input component in-flight
            else if (mInputScript && mInputScriptConstructionTask.getState() == TaskState::TASK_STATE_COMPLETED)
            {
                InputPollDataTask* inputPollDataTask  = inputComponent->getPollDataTask();
                if (inputPollDataTask->readyToPush())
                {
                    inputComponent->setCurrentSceneRuntime(this);
                    //taskManager->pushTask(inputPollDataTask);
                    inputPollDataTask->execute();
                }

                // Process Input
                InputExecuteScriptTask* inputExecuteTask = inputComponent->getExecuteScriptTask();
                if (inputExecuteTask->readyToPush())
                {
                    inputExecuteTask->dependsOn(inputPollDataTask);
                    //taskManager->pushTask(inputExecuteTask);
                    inputExecuteTask->execute();
                }
            }

            // Schedule the PhysicsWorld update Task
            PhysicsUpdateWorldTask* physicsWorldUpdateTask = nullptr;
            if (physicsComponent->getEnabled())
            {
                physicsWorldUpdateTask = physicsComponent->getUpdateWorldTask();
                if (physicsWorldUpdateTask->readyToPush())
                {
                    taskManager->pushTask(physicsWorldUpdateTask);
                }
            }

            // Process Entities
            mRootEntityRuntime->applyToAll(
                        function<EntityRuntime*(EntityRuntime*)>
                        ([&](EntityRuntime* rt){
                            LOG_TRACE("SceneRuntime: Pushing tasks of {}", rt->getNameAndUuidString());
                            rt->updateLifetime();
                            // Animation
                            if (rt->hasAnimationRuntime())
                            {
                                AnimationRuntime* anim = rt->getAnimationRuntime();
                                AnimationUpdateTask* ut = anim->getUpdateTask();
                                if (ut->readyToPush())
                                {
                                    taskManager->pushTask(ut);
                                }
                            }
                            // Audio
                            if (rt->hasAudioRuntime())
                            {
                                AudioRuntime* audio = rt->getAudioRuntime();
                                AudioMarkersUpdateTask* ut = audio->getMarkersUpdateTask();
                                if (ut->readyToPush())
                                {
                                    taskManager->pushTask(ut);
                                }
                            }

                            // Physics
                            if (physicsComponent->getEnabled() && rt->hasPhysicsObjectRuntime())
                            {
                                PhysicsObjectRuntime* pObj = rt->getPhysicsObjectRuntime();
                                if (!pObj->isInPhysicsWorld())
                                {
                                    PhysicsAddObjectTask* ut = pObj->getAddObjectTask();
                                    if (ut->readyToPush())
                                    {
                                        if(physicsWorldUpdateTask->getState() != TaskState::TASK_STATE_COMPLETED)
                                        {
                                            ut->dependsOn(physicsWorldUpdateTask);
                                        }
                                        taskManager->pushTask(ut);
                                    }
                                }
                            }
                            // Path
                            if (rt->hasPathRuntime())
                            {
                                PathRuntime* path = rt->getPathRuntime();
                                PathUpdateTask* ut = path->getUpdateTask();

                                if (ut->readyToPush())
                                {
                                    taskManager->pushTask(ut);
                                }
                            }

                            // Scripting
                            if (scriptComponent->getEnabled() && rt->hasScriptRuntime())
                            {
                                ScriptRuntime* script = rt->getScriptRuntime();
                                EntityScriptConstructionTask* load = rt->getScriptConstructionTask();

                                if (load->getState() == TaskState::TASK_STATE_CONSTRUCTED)
                                {
                                    // Don't clear state of load
                                    //taskManager->pushTask(load);
                                    load->execute();
                                }
                                else if (load->getState() == TaskState::TASK_STATE_COMPLETED)
                                {
                                    if (!rt->getScriptInitialised())
                                    {
                                        EntityScriptOnInitTask* init = rt->getScriptOnInitTask();
                                        //taskManager->pushTask(init);
                                        init->execute();
                                    }
                                    else
                                    {
                                        if (rt->hasEvents())
                                        {
                                            EntityScriptOnEventTask* event = rt->getScriptOnEventTask();
                                            if (event->readyToPush())
                                            {
                                                //event->dependsOn(physicsWorldUpdateTask);
                                                //taskManager->pushTask(event);
                                                event->execute();
                                            }
                                        }

                                        EntityScriptOnUpdateTask* update = rt->getScriptOnUpdateTask();
                                        //taskManager->pushTask(update);
                                        update->execute();
                                    }
                                }
                            }

                            // Graphics
                            if (!rt->getHidden() && rt->hasLightRuntime())
                            {
                                graphicsComponent->addToLightQueue(rt);
                            }

                            return static_cast<EntityRuntime*>(nullptr);
                        }));

            if (physicsComponent->getDebug())
            {
                PhysicsDrawDebugTask* drawDebug = physicsComponent->getDrawDebugTask();
                drawDebug->clearState();
                graphicsComponent->pushTask(drawDebug);
            }

            LOG_TRACE("SceneRuntime: Finished {}",__FUNCTION__);
        } dreamElseLockFailed
    }


    void
    SceneRuntime::updateLifetime
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            Time* time = mProjectRuntime->getTime();
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
        } dreamElseLockFailed
    }

    void
    SceneRuntime::setPlayerEntity
    (EntityRuntime* po)
    {
        if(dreamTryLock()) {
            dreamLock();
            mPlayerEntity = po;
        } dreamElseLockFailed
    }

    EntityRuntime*
    SceneRuntime::getPlayerEntity
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mPlayerEntity;
        } dreamElseLockFailed
    }
}

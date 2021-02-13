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
#include "Components/Script/ScriptDefinition.h"
#include "Components/Script/ScriptRuntime.h"
#include "Project/ProjectRuntime.h"
#include "Components/Cache.h"
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
    (ProjectRuntime* project, SceneDefinition* sd)
        : Runtime(sd),
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
          mInputScriptCreateStateTask(make_shared<InputScriptCreateStateTask>(project)),
          mInputScriptRemoveStateTask(make_shared<InputScriptRemoveStateTask>(project))
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
        mState = state;
    }

    Vector3
    SceneRuntime::getGravity
    ()
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
    (UuidType uuid)
    const
    {
        if (mRootEntityRuntime == nullptr)
        {
            return nullptr;
        }

        return mRootEntityRuntime->applyToAll(
                    function<EntityRuntime*(EntityRuntime*)>
                    ([&](EntityRuntime* currentRuntime)
                    {
                        if (currentRuntime == nullptr)
                        {
                            return static_cast<EntityRuntime*>(nullptr);
                        }
                        else if (currentRuntime->hasUuid(uuid))
                        {
                            return currentRuntime;
                        }
                        return static_cast<EntityRuntime*>(nullptr);
                    }));
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

        return mRootEntityRuntime->applyToAll(
                    function<EntityRuntime*(EntityRuntime*)>
                    ([&](EntityRuntime* currentRuntime)
                    {
                        if (currentRuntime == nullptr)
                        {
                            return static_cast<EntityRuntime*>(nullptr);
                        }
                        else if (currentRuntime->hasName(name))
                        {
                            return currentRuntime;
                        }
                        return static_cast<EntityRuntime*>(nullptr);
                    }));
    }

    int
    SceneRuntime::countEntityRuntimes
    ()
    const
    {
        int count = 0;

        if (mRootEntityRuntime == nullptr)
        {
            return 0;
        }

        mRootEntityRuntime->applyToAll(
                    function<EntityRuntime*(EntityRuntime*)>([&](EntityRuntime*){
                        count++;
                        return static_cast<EntityRuntime*>(nullptr);
                    }));
        return count;
    }

    void
    SceneRuntime::showScenegraph
    ()
    const
    {
        if (!mRootEntityRuntime)
        {
            LOG_DEBUG( "SceneRuntime: Scenegraph is empty (no root EntityRuntime)" );
            return;
        }

        mRootEntityRuntime->applyToAll(
                    function<EntityRuntime*(EntityRuntime*)>([&](EntityRuntime*){
                        LOG_DEBUG("SceneRuntime: showScenegraph not implemented");
                        //obj->showStatus();
                        return nullptr;
                    }));
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
    {
        return mRootEntityRuntime;
    }

    void
    SceneRuntime::collectGarbage
    ()
    {
        LOG_DEBUG( "SceneRuntime: Collecting Garbage {}" , getNameAndUuidString() );
        if (mRootEntityRuntime != nullptr)
        {
            mRootEntityRuntime->applyToAll(
                        function<EntityRuntime*(EntityRuntime*)>([&](EntityRuntime* runt)
                        {
                            runt->collectGarbage();
                            return static_cast<EntityRuntime*>(nullptr);
                        }));
        }
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
    SceneRuntime::loadFromDefinition
    ()
    {
        SceneDefinition* sceneDefinition = static_cast<SceneDefinition*>(mDefinitionHandle);

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
        auto shaderCache = mProjectRuntime->getShaderCache();

        UuidType lightPassShaderUuid = sceneDefinition->getLightingPassShader();
        mLightingPassShader = shaderCache->getRuntimeHandle(lightPassShaderUuid);

        if (mLightingPassShader == nullptr)
        {
            LOG_ERROR("SceneRuntime: Unable to load lighting shader {} for Scene {}",lightPassShaderUuid,getNameAndUuidString());
            return false;
        }

        UuidType shadowPassShaderUuid = sceneDefinition->getShadowPassShader();
        mShadowPassShader = static_cast<ShaderRuntime*>(shaderCache->getRuntimeHandle(shadowPassShaderUuid));

        if (mShadowPassShader == nullptr)
        {
            LOG_ERROR("SceneRuntime: Unable to load shadow shader {} for Scene {}",shadowPassShaderUuid,getNameAndUuidString());
            return false;
        }

        UuidType fontShaderUuid = sceneDefinition->getFontShader();
        mFontShader = static_cast<ShaderRuntime*>(shaderCache->getRuntimeHandle(fontShaderUuid));

        if (mFontShader == nullptr)
        {
            LOG_ERROR("SceneRuntime: Unable to load font shader {} for Scene {}",fontShaderUuid,getNameAndUuidString());
            return false;
        }

        UuidType spriteShaderUuid = sceneDefinition->getSpriteShader();
        mSpriteShader = static_cast<ShaderRuntime*>(shaderCache->getRuntimeHandle(spriteShaderUuid));

        if (mSpriteShader == nullptr)
        {
            LOG_ERROR("SceneRuntime: Unable to load sprite shader {} for Scene {}",spriteShaderUuid,getNameAndUuidString());
            return false;
        }

        mProjectRuntime->getGraphicsComponent()->logShaders();

        // Scripts
        auto scriptCache = mProjectRuntime->getScriptCache();
        UuidType inputScriptUuid = sceneDefinition->getInputScript();
        mInputScript = scriptCache->getRuntimeHandle(inputScriptUuid);
        if (mInputScript)
        {
            LOG_TRACE("SceneRuntime: Setting up input script Tasks");
            mInputScriptCreateStateTask->setScript(mInputScript);
            mInputScriptRemoveStateTask->setScript(mInputScript);
        }
        else
        {
            LOG_ERROR("SceneRuntime: Unable to load Input r Script {}",inputScriptUuid);
        }

        // Physics
        mProjectRuntime->getPhysicsComponent()->setGravity(sceneDefinition->getGravity());

        // Create Root EntityRuntime
        EntityDefinition* entityDef = sceneDefinition->getRootEntityDefinition();
        EntityRuntime* er = new EntityRuntime(mProjectRuntime, this, entityDef);
        if (!er->loadFromDefinition())
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
    ()
    const
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
        if (mRootEntityRuntime != nullptr)
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
    }

    vector<EntityRuntime*>
    SceneRuntime::getEntitysWithRuntimeOf
    (AssetDefinition* def)
    const
    {
        vector<EntityRuntime*> runtimes;
        if (mRootEntityRuntime != nullptr)
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

    ShaderRuntime* SceneRuntime::getFontShader()
    const
    {
        return mFontShader;
    }

    void SceneRuntime::setFontShader(ShaderRuntime* fontShader)
    {
        mFontShader = fontShader;
    }

    ShaderRuntime* SceneRuntime::getSpriteShader()
    const
    {
        return mSpriteShader;
    }

    void SceneRuntime::setSpriteShader(ShaderRuntime* shader)
    {
        mSpriteShader = shader;
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
        if (mRootEntityRuntime != nullptr)
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

    }

    unsigned long
    SceneRuntime::getSceneCurrentTime
    ()
    const
    {
        return mSceneCurrentTime;
    }

    void SceneRuntime::setSceneCurrentTime(unsigned long sceneCurrentTime)
    {
        mSceneCurrentTime = sceneCurrentTime;
    }

    unsigned long
    SceneRuntime::getSceneStartTime
    ()
    const
    {
        return mSceneStartTime;
    }

    void
    SceneRuntime::setSceneStartTime
    (unsigned long sceneStartTime)
    {
        mSceneStartTime = sceneStartTime;
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
        LOG_DEBUG("SceneRuntime: Building SceneRuntime Task Queue...");

        updateLifetime();

        if (mRootEntityRuntime != nullptr)
        {
            // Process Entities
            mRootEntityRuntime->applyToAll(
                        function<EntityRuntime*(EntityRuntime*)>
                        ([&](EntityRuntime* rt){
                            rt->updateLifetime();
                            rt->pushTasks();
                            return static_cast<EntityRuntime*>(nullptr);
                        }));
        }
        LOG_TRACE("SceneRuntime: Finished {}",__FUNCTION__);
    }

    void
    SceneRuntime::updateLifetime
    ()
    {
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
    }

    void
    SceneRuntime::setPlayerEntity
    (EntityRuntime* po)
    {
        mPlayerEntity = po;
    }

    EntityRuntime*
    SceneRuntime::getPlayerEntity
    ()
    const
    {
        return mPlayerEntity;
    }


    bool SceneRuntime::hasState(SceneState state) const
    {
        return mState == state;
    }
}

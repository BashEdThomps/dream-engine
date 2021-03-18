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
#include "Components/Graphics/CameraRuntime.h"
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
using std::static_pointer_cast;

namespace octronic::dream
{
    SceneRuntime::SceneRuntime
    (const weak_ptr<ProjectRuntime>& project,
     const weak_ptr<SceneDefinition>& sd)
        : DeferredLoadRuntime(project, sd),
          mProjectRuntime(project),
          mState(SceneState::SCENE_STATE_TO_LOAD),
          mClearColor(vec4(0.0f)),
          mSceneStartTime(0),
          mSceneCurrentTime(0)
    {
        LOG_TRACE( "SceneRuntime: Constructing " );
    }

    SceneRuntime::~SceneRuntime
    ()
    {
        LOG_TRACE("SceneRuntime: Destructing");
    }

    void
    SceneRuntime::destroyRuntime
    ()
    {
        LOG_DEBUG("SceneRuntime: Destroying runtime {}",getNameAndUuidString());

        if (auto prLock = mProjectRuntime.lock())
        {
            if (prLock->getActiveSceneRuntime().lock() == static_pointer_cast<SceneRuntime>(shared_from_this()))
            {
                if(auto inputComp = prLock->getInputComponent().lock())
                {
                    if (auto destructionQueue = prLock->getDestructionTaskQueue().lock())
                    {
                        destructionQueue->pushTask(inputComp->getRemoveScriptTask());
                    }
                }
            }
        }
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

    vec3
    SceneRuntime::getGravity
    ()
    const
    {
        if (auto prLock = mProjectRuntime.lock())
        {
            if (auto pcLock = prLock->getPhysicsComponent().lock())
            {
                return pcLock->getGravity();
            }
        }
        return vec3(0.0f);
    }

    void
    SceneRuntime::setGravity
    (const vec3& gravity)
    {
        if (auto prLock = mProjectRuntime.lock())
        {
            if (auto pcLock = prLock->getPhysicsComponent().lock())
            {
                pcLock->setGravity(gravity);
            }
        }
    }

    vec4
    SceneRuntime::getClearColor
    ()
    const
    {
        return mClearColor;
    }

    void
    SceneRuntime::setClearColor
    (const vec4& clearColour)
    {
        mClearColor = clearColour;
    }

    weak_ptr<EntityRuntime>
    SceneRuntime::getEntityRuntimeByUuid
    (UuidType uuid)
    const
    {
        for (auto er : mFlatVector)
        {
            if (auto erLock = er.lock())
            {
                if (erLock->hasUuid(uuid)) return er;
            }
        }

        return weak_ptr<EntityRuntime>();
    }

    weak_ptr<EntityRuntime>
    SceneRuntime::getEntityRuntimeByName
    (const string& name)
    const
    {
        for (auto er : mFlatVector)
        {
            if (auto erLock = er.lock())
            {
                if (erLock->hasName(name)) return er;
            }
        }
        return weak_ptr<EntityRuntime>();
    }

    int
    SceneRuntime::countEntityRuntimes
    ()
    const
    {
        return mFlatVector.size();
    }

    void
    SceneRuntime::showScenegraph
    ()
    const
    {
        for (auto er : mFlatVector)
        {
            if (auto erLock = er.lock())
            {
                LOG_TRACE("SceneRuntime: {}", erLock->getNameAndUuidString());
            }
        }
    }

    void
    SceneRuntime::setRootEntityRuntime
    (const shared_ptr<EntityRuntime>& root)
    {
        mRootEntityRuntime = root;
    }

    weak_ptr<EntityRuntime>
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
        LOG_DEBUG( "SceneRuntime: Collecting Garbage {}" , getNameAndUuidString() );
        for (auto er : mFlatVector)
        {
            if (auto erLock = er.lock()) erLock->collectGarbage();
        }
    }

    weak_ptr<ProjectRuntime>
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
        if (auto projRuntLock = mProjectRuntime.lock())
        {
            if (auto sceneDefLock = mDefinition.lock())
            {
                auto sceneDefinition = static_pointer_cast<SceneDefinition>(sceneDefLock);

                LOG_DEBUG( "SceneRuntime: Using SceneDefinition ",  sceneDefinition->getNameAndUuidString() );

                // Assign Runtime attributes from Definition
                setName(sceneDefinition->getName());
                setUuid(sceneDefinition->getUuid());
                setClearColor(sceneDefinition->getClearColor());

                if (auto shaderCache = projRuntLock->getShaderCache().lock())
                {
                    // Load Scene-level Shaders & Textures

                    UuidType shadowPassShaderUuid = sceneDefinition->getShadowPassShader();
                    mShadowPassShader = shaderCache->getRuntime(shadowPassShaderUuid);

                    if (mShadowPassShader.expired())
                    {
                        LOG_ERROR("SceneRuntime: Unable to load shadow shader {} for Scene {}",shadowPassShaderUuid,getNameAndUuidString());
                        return false;
                    }

                    UuidType fontShaderUuid = sceneDefinition->getFontShader();
                    mFontShader = shaderCache->getRuntime(fontShaderUuid);

                    if (mFontShader.expired())
                    {
                        LOG_ERROR("SceneRuntime: Unable to load font shader {} for Scene {}",fontShaderUuid,getNameAndUuidString());
                        return false;
                    }

                    UuidType spriteShaderUuid = sceneDefinition->getSpriteShader();
                    mSpriteShader = shaderCache->getRuntime(spriteShaderUuid);

                    if (mSpriteShader.expired())
                    {
                        LOG_ERROR("SceneRuntime: Unable to load sprite shader {} for Scene {}",spriteShaderUuid,getNameAndUuidString());
                        return false;
                    }

                    UuidType environmentShaderUuid = sceneDefinition->getEnvironmentShader();
                    mEnvironmentShader = shaderCache->getRuntime(environmentShaderUuid);

                    if (mSpriteShader.expired())
                    {
                        LOG_ERROR("SceneRuntime: Unable to load Environment shader {} for Scene {}",environmentShaderUuid,getNameAndUuidString());
                        return false;
                    }
                    if (auto gfxCompLock = projRuntLock->getGraphicsComponent().lock())
                    {
                        gfxCompLock->logShaders();
                    }
                }


                if (auto textureCache = projRuntLock->getTextureCache().lock())
                {
                    UuidType environmentTextureUuid = sceneDefinition->getEnvironmentTexture();
                    mEnvironmentTexture = textureCache->getRuntime(environmentTextureUuid);

                    if (mEnvironmentTexture.expired())
                    {
                        LOG_ERROR("SceneRuntime: Unable to Environment Texture {} for Scene {}",environmentTextureUuid,getNameAndUuidString());
                        return false;
                    }
                }

                // Scripts
                if (auto scriptCache = projRuntLock->getScriptCache().lock())
                {
                    UuidType inputScriptUuid = sceneDefinition->getInputScript();
                    mInputScript = scriptCache->getRuntime(inputScriptUuid);

                    if (mInputScript.expired())
                    {
                        LOG_ERROR("SceneRuntime: Unable to load Input Script {}",inputScriptUuid);
                    }
                }

                // Physics
                if (auto pcLock = projRuntLock->getPhysicsComponent().lock())
                {
                    pcLock->setGravity(sceneDefinition->getGravity());
                }

                // Create Root EntityRuntime
                auto entityDef = sceneDefinition->getRootEntityDefinition();
                auto er = make_shared<EntityRuntime>(mProjectRuntime, static_pointer_cast<SceneRuntime>(shared_from_this()), entityDef);

                if (!er->loadFromDefinition())
                {
                    LOG_ERROR("SceneRuntime: Error using scene object runtime definition");
                    return false;
                }

                setRootEntityRuntime(er);

                // Setup Camera
                mCamera = make_shared<CameraRuntime>(sceneDefinition->getCamera(), static_pointer_cast<SceneRuntime>(shared_from_this()));
                mCamera->loadFromDefinition();

                setState(SceneState::SCENE_STATE_LOADED);

                return true;
            }
        }
        return false;
    }

    vector<weak_ptr<AssetRuntime>>
    SceneRuntime::getAssetRuntimes
    (AssetType t)
    const
    {
        vector<weak_ptr<AssetRuntime>> runtimes;
        for (auto er : mFlatVector)
        {
            if (auto erLock = er.lock())
            {
                auto inst = erLock->getAssetRuntime(t);
                if (!inst.expired())
                {
                    runtimes.push_back(inst);
                }
            }
        }
        return runtimes;
    }

    vector<weak_ptr<EntityRuntime>>
    SceneRuntime::getEntitiesWithRuntimeOf
    (const weak_ptr<AssetDefinition>& def)
    const
    {
        vector<weak_ptr<EntityRuntime>> runtimes;
        if (auto defLock = def.lock())
        {
            auto type = defLock->getAssetType();
            for (auto er : mFlatVector)
            {
                if (auto erLock = er.lock())
                {
                    auto inst = erLock->getAssetRuntime(type);
                    if (auto instLock = inst.lock())
                    {
                        if (instLock->getUuid() == defLock->getUuid()) runtimes.push_back(er);
                    }
                }
            }
        }
        return runtimes;
    }

    weak_ptr<CameraRuntime>
    SceneRuntime::getCamera
    ()
    const
    {
        return mCamera;
    }

    weak_ptr<ShaderRuntime>
    SceneRuntime::getShadowPassShader
    ()
    const
    {
        return mShadowPassShader;
    }

    void
    SceneRuntime::setShadowPassShader
    (const weak_ptr<ShaderRuntime>& shadowPassShader)
    {
        mShadowPassShader = shadowPassShader;
    }

    weak_ptr<ShaderRuntime>
    SceneRuntime::getFontShader
    ()
    const
    {
        return mFontShader;
    }

    void
    SceneRuntime::setFontShader
    (const weak_ptr<ShaderRuntime>& fontShader)
    {
        mFontShader = fontShader;
    }

    weak_ptr<ShaderRuntime>
    SceneRuntime::getSpriteShader
    ()
    const
    {
        return mSpriteShader;
    }

    void
    SceneRuntime::setSpriteShader
    (const weak_ptr<ShaderRuntime>& shader)
    {
        mSpriteShader = shader;
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
     * All SharedAssetRuntimes should be pushed by their caches.
     *
     */
    void
    SceneRuntime::createSceneTasks
    ()
    {
        LOG_DEBUG("SceneRuntime: Building SceneRuntime Task Queue...");

        updateLifetime();

        for (auto er : mFlatVector)
        {
            if (auto erLock = er.lock())
            {
                erLock->pushTasks();
            }
        }
        LOG_TRACE("SceneRuntime: Finished {}",__FUNCTION__);
    }

    void
    SceneRuntime::updateFlatVector
    ()
    {
        mFlatVector.clear();
        if (mRootEntityRuntime)
        {
            mFlatVector = mRootEntityRuntime->generateFlatVector();
        }
    }

    void
    SceneRuntime::updateLifetime
    ()
    {
        if (auto prLock = mProjectRuntime.lock())
        {
            auto time = prLock->getTime().lock();
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

    bool
    SceneRuntime::hasState
    (SceneState state) const
    {
        return mState == state;
    }

    weak_ptr<TextureRuntime>
    SceneRuntime::getEnvironmentTexture
    () const
    {
        return mEnvironmentTexture;
    }

    void
    SceneRuntime::setEnvironmentTexture
    (const weak_ptr<TextureRuntime>& tr)
    {
        mEnvironmentTexture = tr;;
    }

    weak_ptr<ShaderRuntime>
    SceneRuntime::getEnvironmentShader
    () const
    {
        return mEnvironmentShader;
    }

    void
    SceneRuntime::setEnvironmentShader
    (const weak_ptr<ShaderRuntime>& rt)
    {
        mEnvironmentShader = rt;
    }

    weak_ptr<ScriptRuntime>
    SceneRuntime::getInputScript
    () const
    {
        return mInputScript;
    }

    vector<weak_ptr<EntityRuntime>>
    SceneRuntime::getFlatVector
    () const
    {
        return mFlatVector;
    }
}

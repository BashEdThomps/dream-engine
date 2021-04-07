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
#include "Entity/TemplateEntityDefinition.h"
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
#include "Components/Physics/PhysicsRuntime.h"
#include "Components/Path/PathRuntime.h"
#include "Components/Script/ScriptDefinition.h"
#include "Components/Script/ScriptRuntime.h"
#include "Project/ProjectRuntime.h"
#include "Components/Cache.h"

#include <iostream>
#include <exception>

using std::exception;

namespace octronic::dream
{
  SceneRuntime::SceneRuntime
  (ProjectRuntime& project,
   SceneDefinition& sd)
    : DeferredLoadRuntime(project, sd),
      mState(SceneState::SCENE_STATE_TO_LOAD),
      mClearColor(vec4(0.0f)),
      mCameraRuntime(*this, sd.getCameraDefinition()),
      mSceneStartTime(0),
      mSceneCurrentTime(0)
  {
    LOG_TRACE( "SceneRuntime: Constructing " );
  }

  void
  SceneRuntime::destroyRuntime
  ()
  {
    LOG_DEBUG("SceneRuntime: Destroying runtime {}",getNameAndUuidString());

    auto activeSrOpt = getProjectRuntime().getActiveSceneRuntime();

    if (activeSrOpt)
    {
      auto& activeSr = activeSrOpt.value().get();

      if (activeSr == *this)
      {
        auto& inputComp = getProjectRuntime().getInputComponent();
        auto& destructionQueue = getProjectRuntime().getDestructionTaskQueue();
        destructionQueue.pushTask(inputComp.getRemoveScriptTask());
      }
      mState = SceneState::SCENE_STATE_DESTROYED;
    }
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
  {
    auto& physicsComp = getProjectRuntime().getPhysicsComponent();
    return physicsComp.getGravity();
  }

  void
  SceneRuntime::setGravity
  (const vec3& gravity)
  {
    auto& physicsComp = getProjectRuntime().getPhysicsComponent();
    physicsComp.setGravity(gravity);
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

  EntityRuntime&
  SceneRuntime::getEntityRuntimeByUuid
  (UuidType uuid)
  {
    for (auto& er : mFlatVector)
    {
      if (er.get().hasUuid(uuid)) return er;
    }
    throw std::exception();
  }

  EntityRuntime&
  SceneRuntime::getEntityRuntimeByName
  (const string& name)
  {
    for (auto& er : mFlatVector)
    {
      if (er.get().hasName(name)) return er;
    }
    throw std::exception();
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
    for (auto& er : mFlatVector)
    {
      LOG_TRACE("SceneRuntime: {}", er.get().getNameAndUuidString());
    }
  }

  EntityRuntime&
  SceneRuntime::getRootEntityRuntime
  ()
  {
    return mRootEntityRuntime.value();
  }

  void
  SceneRuntime::collectGarbage
  ()
  {
    LOG_DEBUG( "SceneRuntime: Collecting Garbage {}" , getNameAndUuidString() );
    for (auto& er : mFlatVector)
    {
      er.get().collectGarbage();
    }
  }

  bool
  SceneRuntime::loadFromDefinition
  ()
  {
    auto& pr = getProjectRuntime();
    auto& sceneDefinition = static_cast<SceneDefinition&>(getDefinition());

    LOG_DEBUG( "SceneRuntime: Using SceneDefinition ",  sceneDefinition.getNameAndUuidString() );

    // Assign Runtime attributes from Definition
    setName(sceneDefinition.getName());
    setUuid(sceneDefinition.getUuid());
    setClearColor(sceneDefinition.getClearColor());

    auto& pDef = static_cast<ProjectDefinition&>(pr.getDefinition());
    auto& shaderCache = pr.getShaderCache();
    // Load Scene-level Shaders & Textures

    UuidType shadowPassShaderUuid = sceneDefinition.getShadowPassShader();
    auto shadowPassDef = pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_SHADER, shadowPassShaderUuid);
    if (shadowPassDef)
    {
    	mShadowPassShader = shaderCache.getRuntime(static_cast<ShaderDefinition&>(shadowPassDef.value().get()));
    }

    if (!mShadowPassShader)
    {
      LOG_ERROR("SceneRuntime: Unable to load shadow shader {} for Scene {}",shadowPassShaderUuid,getNameAndUuidString());
      return false;
    }

    UuidType fontShaderUuid = sceneDefinition.getFontShader();
    auto fontShaderDef = pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_SHADER, fontShaderUuid);
    if (fontShaderDef)
    {
    	mFontShader = shaderCache.getRuntime(static_cast<ShaderDefinition&>(fontShaderDef.value().get()));
    }

    if (!mFontShader)
    {
      LOG_ERROR("SceneRuntime: Unable to load font shader {} for Scene {}",fontShaderUuid,getNameAndUuidString());
      return false;
    }

    UuidType spriteShaderUuid = sceneDefinition.getSpriteShader();
    auto spriteShaderDef = pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_SHADER, spriteShaderUuid);
    if (spriteShaderDef)
    {
    	mSpriteShader = shaderCache.getRuntime(static_cast<ShaderDefinition&>(spriteShaderDef.value().get()));
    }

    if (!mSpriteShader)
    {
      LOG_ERROR("SceneRuntime: Unable to load sprite shader {} for Scene {}",spriteShaderUuid,getNameAndUuidString());
      return false;
    }

    UuidType environmentShaderUuid = sceneDefinition.getEnvironmentShader();
    auto envShaderDef = pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_SHADER, environmentShaderUuid);
    if (envShaderDef)
    {
    	mEnvironmentShader = shaderCache.getRuntime(static_cast<ShaderDefinition&>(envShaderDef.value().get()));
    }

    if (!mSpriteShader)
    {
      LOG_ERROR("SceneRuntime: Unable to load Environment shader {} for Scene {}",environmentShaderUuid,getNameAndUuidString());
      return false;
    }

    auto& textureCache = pr.getTextureCache();
    UuidType environmentTextureUuid = sceneDefinition.getEnvironmentTexture();
    auto envTexDef = pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_TEXTURE, environmentTextureUuid);
    if (envTexDef)
    {
    	mEnvironmentTexture = textureCache.getRuntime(static_cast<TextureDefinition&>(envTexDef.value().get()));
    }

    if (!mEnvironmentTexture)
    {
      LOG_ERROR("SceneRuntime: Unable to Environment Texture {} for Scene {}",environmentTextureUuid,getNameAndUuidString());
      return false;
    }

    // Scripts
    auto& scriptCache = pr.getScriptCache();
    UuidType inputScriptUuid = sceneDefinition.getInputScript();
    auto scriptDef = pDef.getAssetDefinitionByUuid(ASSET_TYPE_ENUM_SCRIPT,inputScriptUuid);
    if (scriptDef)
    {
    	mInputScript = scriptCache.getRuntime(static_cast<ScriptDefinition&>(scriptDef.value().get()));
    }

    if (!mInputScript)
    {
      LOG_ERROR("SceneRuntime: Unable to load Input Script {}",inputScriptUuid);
    }

    // Physics
    auto& pc = pr.getPhysicsComponent();
    pc.setGravity(sceneDefinition.getGravity());

    // Create Root EntityRuntime
    try
    {
    	auto& entityDef = sceneDefinition.getRootSceneEntityDefinition();
    	auto templateDef = pDef.getTemplateEntityDefinitionByUuid(entityDef.value().getTemplateUuid());
      mRootEntityRuntime.emplace(getProjectRuntime(), *this, entityDef.value(), templateDef.value().get());

      if (!mRootEntityRuntime.value().loadFromDefinition())
      {
        LOG_ERROR("SceneRuntime: Error using scene object runtime definition");
        return false;
      }
    }
    catch (exception& ex)
    {
      LOG_ERROR("SceneRuntime: Error while loading, no root entity definition found");
      setState(SCENE_STATE_TO_DESTROY);
    	return false;
    }

    // Setup Camera
    mCameraRuntime.loadFromDefinition();

    setState(SceneState::SCENE_STATE_LOADED);

    return true;
  }

  vector<reference_wrapper<AssetRuntime>>
  SceneRuntime::getAssetRuntimes
  (AssetType type)
  const
  {
    vector<reference_wrapper<AssetRuntime>> runtimes;
    for (auto& erRef : mFlatVector)
    {
      auto& er = erRef.get();
      runtimes.push_back(er.getAssetRuntime(type));
    }
    return runtimes;
  }

  vector<reference_wrapper<EntityRuntime>>
  SceneRuntime::getEntitiesWithRuntimeOf
  (AssetDefinition& def)
  const
  {
    vector<reference_wrapper<EntityRuntime>> runtimes;
    auto type = def.getAssetType();
    for (auto erRef : mFlatVector)
    {
      auto& er = erRef.get();
      auto& inst = er.getAssetRuntime(type);
      if (inst.getUuid() == def.getUuid()) runtimes.push_back(er);
    }
    return runtimes;
  }

  CameraRuntime&
  SceneRuntime::getCameraRuntime
  ()
  {
    return mCameraRuntime;
  }

  ShaderRuntime&
  SceneRuntime::getShadowPassShader
  ()
  {
    return mShadowPassShader.value();
  }

  void
  SceneRuntime::setShadowPassShader
  (ShaderRuntime& shadowPassShader)
  {
    mShadowPassShader = shadowPassShader;
  }

  ShaderRuntime&
  SceneRuntime::getFontShader
  ()
  {
    return mFontShader.value();
  }

  void
  SceneRuntime::setFontShader
  (ShaderRuntime& fontShader)
  {
    mFontShader = fontShader;
  }

  ShaderRuntime&
  SceneRuntime::getSpriteShader
  ()
  {
    return mSpriteShader.value();
  }

  void
  SceneRuntime::setSpriteShader
  (ShaderRuntime& shader)
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

    for (auto& er : mFlatVector)
    {
      er.get().pushTasks();
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
      mFlatVector = mRootEntityRuntime.value().generateFlatVector();
    }
  }

  void
  SceneRuntime::updateLifetime
  ()
  {
    auto& pr = getProjectRuntime();
    auto& time = pr.getTime();

    long timeDelta = time.getFrameTimeDelta();

    if (timeDelta <= Time::DELTA_MAX)
    {
      long frameTime = time.getCurrentFrameTime();
      if (getSceneStartTime() <= 0)
      {
        setSceneStartTime(frameTime);
      }
      setSceneCurrentTime(frameTime-getSceneStartTime());
    }
  }

  bool
  SceneRuntime::hasState
  (SceneState state) const
  {
    return mState == state;
  }

  TextureRuntime&
  SceneRuntime::getEnvironmentTexture
  ()
  {
    return mEnvironmentTexture.value();
  }

  void
  SceneRuntime::setEnvironmentTexture
  (TextureRuntime& tr)
  {
    mEnvironmentTexture = tr;
  }

  ShaderRuntime&
  SceneRuntime::getEnvironmentShader
  ()
  {
    return mEnvironmentShader.value();
  }

  void
  SceneRuntime::setEnvironmentShader
  (ShaderRuntime& rt)
  {
    mEnvironmentShader = rt;
  }

  ScriptRuntime&
  SceneRuntime::getInputScript
  ()
  {
    return mInputScript.value();
  }

  vector<reference_wrapper<EntityRuntime>>
  SceneRuntime::getFlatVector
  () const
  {
    return mFlatVector;
  }
}

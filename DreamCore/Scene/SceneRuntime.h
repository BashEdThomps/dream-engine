#pragma once

#include "SceneState.h"

#include "Common/AssetType.h"
#include "Base/DeferredLoadRuntime.h"
#include "Math/Transform.h"
#include "Components/Graphics/CameraRuntime.h"
#include "Entity/EntityRuntime.h"

#include <memory>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace octronic::dream
{
  class AssetDefinition;
  class SceneDefinition;
  class AssetRuntime;
  class ProjectRuntime;
  class ScriptRuntime;
  class ShaderRuntime;
  class TextureRuntime;

  class SceneRuntime : public DeferredLoadRuntime
  {
  public:
    SceneRuntime(ProjectRuntime& parent, SceneDefinition& sd);

    SceneRuntime(SceneRuntime&&) = default;
    SceneRuntime& operator=(SceneRuntime&&) = default;

    CameraRuntime& getCameraRuntime();

    SceneState getState() const;
    void setState(SceneState state);
    bool hasState(SceneState state) const;

    vec3 getGravity();
    void setGravity(const vec3& gravity);

    vec4 getClearColor() const;
    void setClearColor(const vec4& clear);

    bool loadFromDefinition() override;
    void destroyRuntime();

    void createSceneTasks();

    optional<reference_wrapper<EntityRuntime>> getRootEntityRuntime();
    optional<reference_wrapper<EntityRuntime>> getEntityRuntimeByName(const string& name) const;
    optional<reference_wrapper<EntityRuntime>> getEntityRuntimeByUuid(UuidType uuid) const;

    int countEntityRuntimes() const;
    int countChildrenOfEntityRuntime(const EntityRuntime&) const;

    void showScenegraph() const;
    void collectGarbage();

    optional<reference_wrapper<ShaderRuntime>> getShadowPassShader() const;
    void setShadowPassShader(ShaderRuntime& shadowPassShader);


    optional<reference_wrapper<ShaderRuntime>> getFontShader() const;
    void setFontShader(ShaderRuntime& shader);

    optional<reference_wrapper<ShaderRuntime>> getSpriteShader() const;
    void setSpriteShader(ShaderRuntime& shader);

    optional<reference_wrapper<TextureRuntime>> getEnvironmentTexture() const;
    void setEnvironmentTexture(TextureRuntime&);

    optional<reference_wrapper<ShaderRuntime>> getEnvironmentShader() const;
    void setEnvironmentShader(ShaderRuntime&);

    vector<reference_wrapper<AssetRuntime>>  getAssetRuntimes(AssetType) const;
    vector<reference_wrapper<EntityRuntime>> getEntitiesWithRuntimeOf(AssetDefinition& def) const;
    vector<reference_wrapper<EntityRuntime>> getFlatVector() const;

    /**
         * @return Gets the nearest Entity to the Camera's position excluding
         * the Entity the Camera is focused on.
         */

    unsigned long getSceneCurrentTime() const;
    void setSceneCurrentTime(unsigned long sceneCurrentTime);

    unsigned long getSceneStartTime() const;
    void setSceneStartTime(unsigned long sceneStartTime);

    ScriptRuntime& getInputScript();

    void updateFlatVector();

  protected:
    void updateLifetime();
    void createAllAssetRuntimes();
    void setDeleteFlagOnAllEntityRuntimes();

  private:
    SceneState mState;
    vec4 mClearColor;
    vector<reference_wrapper<EntityRuntime>> mEntityRuntimeCleanUpQueue;
    optional<EntityRuntime> mRootEntityRuntime;
    vector<reference_wrapper<EntityRuntime>> mFlatVector;
    optional<reference_wrapper<ShaderRuntime>> mShadowPassShader;
    optional<reference_wrapper<ShaderRuntime>> mFontShader;
    optional<reference_wrapper<ShaderRuntime>> mSpriteShader;
    optional<reference_wrapper<TextureRuntime>> mEnvironmentTexture;
    optional<reference_wrapper<ShaderRuntime>> mEnvironmentShader;
    optional<reference_wrapper<ScriptRuntime>> mInputScript;
    CameraRuntime mCameraRuntime;
    unsigned long mSceneStartTime;
    unsigned long mSceneCurrentTime;
  };
}

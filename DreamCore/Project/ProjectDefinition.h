#pragma once

#include "Base/Definition.h"
#include "Components/AssetDefinition.h"
#include "Components/Animation/AnimationDefinition.h"
#include "Components/Audio/AudioDefinition.h"
#include "Components/Graphics/Font/FontDefinition.h"
#include "Components/Graphics/Material/MaterialDefinition.h"
#include "Components/Graphics/Model/ModelDefinition.h"
#include "Components/Path/PathDefinition.h"
#include "Components/Physics/PhysicsDefinition.h"
#include "Components/Graphics/Shader/ShaderDefinition.h"
#include "Components/Script/ScriptDefinition.h"
#include "Components/Graphics/Texture/TextureDefinition.h"
#include "Scene/SceneDefinition.h"
#include "Entity/TemplateEntityDefinition.h"

#include <memory>
#include <vector>

using std::string;
using std::vector;
using std::function;
using std::reference_wrapper;
using std::unique_ptr;

namespace octronic::dream
{
  class ProjectContext;

  class ProjectDefinition : public Definition
  {
  public:
    ProjectDefinition(const json& data);
    ProjectDefinition(ProjectDefinition&&) = default;
    ProjectDefinition& operator=(ProjectDefinition&&) = default;

    // Assets ==============================================================

    AssetDefinition& createAssetDefinition(AssetType);
    size_t countAssetDefinitions(AssetType);
    optional<reference_wrapper<AssetDefinition>> getAssetDefinitionByUuid(AssetType, UuidType uuid);
    optional<reference_wrapper<AssetDefinition>> getAssetDefinitionByName(AssetType, const string& name);
    optional<reference_wrapper<AssetDefinition>> getAssetDefinitionAtIndex(AssetType, int);
    vector<string> getAssetNamesVector(AssetType);
    vector<reference_wrapper<AssetDefinition>> getAssetDefinitionsVector(AssetType);
    int getAssetDefinitionIndex(AssetDefinition&);
    void removeAssetDefinitionByUuid(AssetType, UuidType);
    void removeAssetDefinition(AssetDefinition& assetDef);
    void removeAllAssetDefinitions();

    // Scenes ==============================================================

    SceneDefinition& createSceneDefinition();
    UuidType getStartupSceneUuid() const;
    optional<reference_wrapper<SceneDefinition>> getStartupSceneDefinition();
    optional<reference_wrapper<SceneDefinition>> getSceneDefinitionByUuid(UuidType uuid);
    optional<reference_wrapper<SceneDefinition>> getSceneDefinitionByName(const string& name);
    vector<reference_wrapper<SceneDefinition>> getSceneDefinitionsVector() const;
    SceneDefinition& getSceneDefinitionAtIndex(int index);
    vector<string> getSceneDefinitionNamesVector() const;
    int getSceneDefinitionIndex(SceneDefinition&) const;
    size_t countScenesDefinitions() const;
    void setStartupSceneUuid(UuidType sceneUuid);
    void removeSceneDefinition(SceneDefinition& sceneDef);
    void removeAllSceneDefinitions();

    // Template Entity Management ==========================================

    TemplateEntityDefinition& createTemplateEntityDefinition();
    optional<reference_wrapper<TemplateEntityDefinition>> getTemplateEntityDefinitionByUuid(UuidType uuid);
    vector<string> getTemplateEntityDefinitionNamesVector();
    void removeTemplateEntityDefinitionByUuid(UuidType);
    void removeTemplateEntityDefinition(TemplateEntityDefinition&);
    vector<reference_wrapper<TemplateEntityDefinition>> getTemplateEntityDefinitionsVector() const;
    int getTemplateEntityDefinitionIndex(TemplateEntityDefinition& def);
    TemplateEntityDefinition& getTemplateEntityDefinitionAtIndex(int index);

    json getJson() override;

  private:
    void loadAllSceneDefinitions();
    void loadAllAssetDefinitions();
    void loadTemplateEntityDefinitions();
    void loadSingleAssetDefinition(const json& assetDefinition);

  private:
    // Scenes
    vector<unique_ptr<SceneDefinition>> mSceneDefinitions;
    // Template Entities
    vector<unique_ptr<TemplateEntityDefinition>> mTemplateEntityDefinitions;
    // Assets
    vector<unique_ptr<AnimationDefinition>> mAnimationDefinitions;
    vector<unique_ptr<AudioDefinition>> mAudioDefinitions;
    vector<unique_ptr<FontDefinition>> mFontDefinitions;
    vector<unique_ptr<MaterialDefinition>> mMaterialDefinitions;
    vector<unique_ptr<ModelDefinition>> mModelDefinitions;
    vector<unique_ptr<PathDefinition>> mPathDefinitions;
    vector<unique_ptr<PhysicsDefinition>> mPhysicsDefinitions;
    vector<unique_ptr<ScriptDefinition>> mScriptDefinitions;
    vector<unique_ptr<ShaderDefinition>> mShaderDefinitions;
    vector<unique_ptr<TextureDefinition>> mTextureDefinitions;
  };
}

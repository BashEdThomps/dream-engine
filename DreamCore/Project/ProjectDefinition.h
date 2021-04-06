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

#pragma once

#include "Base/Definition.h"
#include "Components/AssetDefinition.h"
#include "Components/Animation/AnimationDefinition.h"
#include "Components/Audio/AudioDefinition.h"
#include "Components/Graphics/Font/FontDefinition.h"
#include "Components/Graphics/Material/MaterialDefinition.h"
#include "Components/Graphics/Model/ModelDefinition.h"
#include "Components/Path/PathDefinition.h"
#include "Components/Physics/PhysicsObjectDefinition.h"
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

namespace octronic::dream
{
  class ProjectContext;

  class ProjectDefinition : public Definition
  {
  public:
    ProjectDefinition(const string& name);
    ProjectDefinition(const json& data);
    ProjectDefinition(ProjectDefinition&&) = default;
    ProjectDefinition& operator=(ProjectDefinition&&) = default;

    // Assets ==============================================================

    AssetDefinition& createNewAssetDefinition(AssetType);
    size_t countAssetDefinitions(AssetType);
    optional<reference_wrapper<AssetDefinition>> getAssetDefinitionByPredicate(AssetType, function<bool(AssetDefinition&)> fn);
    optional<reference_wrapper<AssetDefinition>> getAssetDefinitionByUuid(AssetType, UuidType uuid);
    optional<reference_wrapper<AssetDefinition>> getAssetDefinitionByName(AssetType, const string& name);
    optional<reference_wrapper<AssetDefinition>> getAssetDefinitionAtIndex(AssetType, int);
    vector<string> getAssetNamesVector(AssetType);
    vector<reference_wrapper<AssetDefinition>> getAssetDefinitionsVector(AssetType);
    unsigned long getAssetDefinitionIndex(AssetDefinition&);
    void removeAssetDefinition(AssetDefinition& assetDef);
    void deleteAllAssetDefinitions();

    // Scenes ==============================================================

    SceneDefinition& createNewSceneDefinition();
    UuidType getStartupSceneUuid() const;
    optional<reference_wrapper<SceneDefinition>> getStartupSceneDefinition();
    optional<reference_wrapper<SceneDefinition>> getSceneDefinitionByUuid(UuidType uuid);
    optional<reference_wrapper<SceneDefinition>> getSceneDefinitionByName(const string& name);
    vector<SceneDefinition>& getSceneDefinitionsVector();
    SceneDefinition& getSceneDefinitionAtIndex(int index);
    long getSceneDefinitionIndex(SceneDefinition&) const;
    size_t countScenesDefinitions() const;
    void setStartupSceneUuid(UuidType sceneUuid);
    void removeSceneDefinition(SceneDefinition& sceneDef);
    void deleteAllSceneDefinitions();

    // Template Entity Management ==========================================

    TemplateEntityDefinition& createTemplateEntityDefinition();
    optional<reference_wrapper<TemplateEntityDefinition>> getTemplateEntityDefinitionByUuid(UuidType uuid);
    vector<string>            getTemplateEntityNamesVector();
    void removeTemplateEntityDefinitionByUuid(UuidType);
    void removeTemplateEntityDefinition(TemplateEntityDefinition&);

    json toJson() override;

  private:
    void loadAllSceneDefinitions();
    void loadAllAssetDefinitions();
    void loadTemplateEntityDefinitions();
    void loadSingleAssetDefinition(const json& assetDefinition);

  private:
    // Scenes
    vector<SceneDefinition> mSceneDefinitions;
    // Template Entities
    vector<TemplateEntityDefinition> mTemplateEntityDefinitions;
    // Assets
    vector<AnimationDefinition> mAnimationDefinitions;
    vector<AudioDefinition> mAudioDefinitions;
    vector<FontDefinition> mFontDefinitions;
    vector<MaterialDefinition> mMaterialDefinitions;
    vector<ModelDefinition> mModelDefinitions;
    vector<PathDefinition> mPathDefinitions;
    vector<PhysicsObjectDefinition> mPhysicsObjectDefinitions;
    vector<ScriptDefinition> mScriptDefinitions;
    vector<ShaderDefinition> mShaderDefinitions;
    vector<TextureDefinition> mTextureDefinitions;
  };
}

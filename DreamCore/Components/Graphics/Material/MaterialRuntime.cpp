#include "MaterialRuntime.h"

#include "MaterialDefinition.h"
#include "Common/Logger.h"
#include "Components/Graphics/Shader/ShaderRuntime.h"
#include "Components/Graphics/Texture/TextureRuntime.h"
#include "Components/Graphics/Model/ModelMesh.h"
#include "Components/Graphics/CameraRuntime.h"
#include "Project/ProjectRuntime.h"
#include "Components/Cache.h"

using std::static_pointer_cast;

namespace octronic::dream
{
  MaterialRuntime::MaterialRuntime(
      ProjectRuntime& rt,
      MaterialDefinition& def)
    : SharedAssetRuntime(rt, def)

  {
    LOG_TRACE("MaterialRuntime: Constructing");
  }

  void
  MaterialRuntime::addMesh
  (ModelMesh& mesh)
  {
    mUsedBy.push_back(mesh);
  }

  void
  MaterialRuntime::removeMesh
  (ModelMesh& mesh)
  {
    auto itr = find_if(mUsedBy.begin(), mUsedBy.end(),
                   [&](reference_wrapper<ModelMesh>& next)
    { return &next.get() == &mesh; });

    if (itr != mUsedBy.end()) mUsedBy.erase(itr);
  }

  void
  MaterialRuntime::clearMeshes
  ()
  {
    mUsedBy.clear();
  }

  size_t
  MaterialRuntime::countMeshes
  ()
  {
    return mUsedBy.size();
  }

  bool
  MaterialRuntime::operator==
  (MaterialRuntime& other)
  {
    return getName() == other.getName();
  }

  void
  MaterialRuntime::debug
  ()
  {
    GLuint albedo, normal, metallic, roughness, ao;
    albedo    = (!mAlbedoTexture.has_value()    ? 0 : mAlbedoTexture.value().get().getTextureID());
    normal    = (!mNormalTexture.has_value()    ? 0 : mNormalTexture.value().get().getTextureID());
    metallic  = (!mMetallicTexture.has_value()  ? 0 : mMetallicTexture.value().get().getTextureID());
    roughness = (!mRoughnessTexture.has_value() ? 0 : mRoughnessTexture.value().get().getTextureID());
    ao        = (!mAoTexture.has_value()        ? 0 : mAoTexture.value().get().getTextureID());

    LOG_TRACE("Maerial Parameters\n"
            "Name....................{}\n"
            "AlbedoTexture...........{}\n"
            "NormalTexture...........{}\n"
            "MetallicTexture.........{}\n"
            "RoughnessTexture........{}\n"
            "AoTexture...............{}\n"
            "Meshes..................{}",
              getName(), albedo, normal,
              metallic, roughness, ao,
              mUsedBy.size());
  }

  void
  MaterialRuntime::logMeshes
  ()
  {
    LOG_DEBUG("\tMeshes for material {} : {}",getName(),mUsedBy.size());
    for (auto& mesh : mUsedBy)
    {
      LOG_DEBUG("\t\t{}", mesh.get().getName());
      mesh.get().logRuntimes();
    }
  }

  vector<reference_wrapper<ModelMesh>>
  MaterialRuntime::getUsedByVector()
  const
  {
    return mUsedBy;
  }

  void
  MaterialRuntime::drawShadowPass
  (ShaderRuntime& shader)
  {
    for (auto& mesh : mUsedBy)
    {
      mesh.get().drawShadowPassRuntimes(shader);
    }
  }

  bool
  MaterialRuntime::loadFromDefinition
  ()
  {
    auto& matDef = static_cast<MaterialDefinition&>(getDefinition());

    // Shaders & Textures
    auto& pRunt = getProjectRuntime();
    auto& pDef = static_cast<ProjectDefinition&>(pRunt.getDefinition());
    auto& shaderCache = pRunt.getShaderCache();
    auto& textureCache = pRunt.getTextureCache();
    auto shaderDef = pDef.getAssetDefinitionByUuid(AssetType::ASSET_TYPE_ENUM_SHADER, matDef.getShaderUuid());
    if (shaderDef)
    {
      mShader = shaderCache.getRuntime(static_cast<ShaderDefinition&>(shaderDef.value().get()));
    }

    if (!mShader)
    {
      LOG_ERROR("MaterialCache: Cannot create material {} with null shader", matDef.getNameAndUuidString());
      return false;
    }

    auto albedoDef = pDef.getAssetDefinitionByUuid(AssetType::ASSET_TYPE_ENUM_TEXTURE, matDef.getAlbedoTextureUuid());
    if (albedoDef)
    {
      mAlbedoTexture = textureCache.getRuntime(static_cast<TextureDefinition&>(albedoDef.value().get()));
    }

    auto normalDef = pDef.getAssetDefinitionByUuid(AssetType::ASSET_TYPE_ENUM_TEXTURE, matDef.getNormalTextureUuid());
    if(normalDef)
    {
      mNormalTexture = textureCache.getRuntime(static_cast<TextureDefinition&>(normalDef.value().get()));
    }

    auto metallicDef = pDef.getAssetDefinitionByUuid(AssetType::ASSET_TYPE_ENUM_TEXTURE, matDef.getMetallicTextureUuid());
    if(metallicDef)
    {
      mMetallicTexture = textureCache.getRuntime(static_cast<TextureDefinition&>(metallicDef.value().get()));
    }

    auto roughnessDef = pDef.getAssetDefinitionByUuid(AssetType::ASSET_TYPE_ENUM_TEXTURE, matDef.getRoughnessTextureUuid());
    if (roughnessDef)
    {
      mRoughnessTexture = textureCache.getRuntime(static_cast<TextureDefinition&>(roughnessDef.value().get()));
    }

    auto aoDef = pDef.getAssetDefinitionByUuid(AssetType::ASSET_TYPE_ENUM_TEXTURE, matDef.getAoTextureUuid());
    if (aoDef)
    {
      mAoTexture = textureCache.getRuntime(static_cast<TextureDefinition&>(aoDef.value().get()));
    }

    mShader.value().get().addMaterial(*this);
    return true;
  }

  optional<reference_wrapper<ShaderRuntime>>
  MaterialRuntime::getShader
  () const
  {
    return mShader;
  }

  void
  MaterialRuntime::setShader
  (ShaderRuntime& shader)
  {
    mShader = optional<reference_wrapper<ShaderRuntime>>(shader);
  }

  optional<reference_wrapper<TextureRuntime>>
  MaterialRuntime::getAlbedoTexture
  () const
  {
    return mAlbedoTexture;
  }

  void
  MaterialRuntime::setAlbedoTexture
  (TextureRuntime& t)
  {
    mAlbedoTexture = optional<reference_wrapper<TextureRuntime>>(t);
  }

  optional<reference_wrapper<TextureRuntime>>
  MaterialRuntime::getNormalTexture
  () const
  {
    return mNormalTexture;
  }

  void
  MaterialRuntime::setNormalTexture
  (TextureRuntime& normalTexture)
  {
    mNormalTexture = optional<reference_wrapper<TextureRuntime>>(normalTexture);
  }

  optional<reference_wrapper<TextureRuntime>>
  MaterialRuntime::getMetallicTexture
  () const
  {
    return mMetallicTexture;
  }

  void
  MaterialRuntime::setMetallicTexture
  (TextureRuntime& t)
  {
    mMetallicTexture = optional<reference_wrapper<TextureRuntime>>(t);
  }

  optional<reference_wrapper<TextureRuntime>>
  MaterialRuntime::getRoughnessTexture
  () const
  {
    return mRoughnessTexture;
  }

  void
  MaterialRuntime::setRoughnessTexture
  (TextureRuntime& t)
  {
    mRoughnessTexture = optional<reference_wrapper<TextureRuntime>>(t);
  }

  optional<reference_wrapper<TextureRuntime>>
  MaterialRuntime::getAoTexture
  () const
  {
    return mAoTexture;
  }

  void
  MaterialRuntime::setAoTexture
  (TextureRuntime& t)
  {
    mAoTexture = optional<reference_wrapper<TextureRuntime>>(t);
  }

  void
  MaterialRuntime::pushTasks
  ()
  {
    auto& taskQueue = getProjectRuntime().getTaskQueue();
    if (!mLoaded && !mLoadError && mLoadFromDefinitionTask->hasState(TASK_STATE_QUEUED))
    {
      taskQueue.pushTask(mLoadFromDefinitionTask);
    }
  }
}

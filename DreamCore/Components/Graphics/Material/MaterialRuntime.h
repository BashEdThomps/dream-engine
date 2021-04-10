#pragma once

#include "Components/SharedAssetRuntime.h"
#include "Components/Graphics/Model/ModelMesh.h"

#include <optional>
#include <vector>
#include <glm/vec3.hpp>

using std::optional;
using std::reference_wrapper;
using std::vector;
using glm::vec3;

namespace octronic::dream
{
  class ShaderRuntime;
  class MaterialDefinition;
  class TextureRuntime;
  class CameraRuntime;

  class MaterialRuntime : public SharedAssetRuntime
  {
  public:
    MaterialRuntime(ProjectRuntime& rt, MaterialDefinition& def);
    MaterialRuntime(MaterialRuntime&&) = default;
    MaterialRuntime& operator=(MaterialRuntime&&) = default;


    void addMesh(ModelMesh& mesh);
    void removeMesh(ModelMesh& mesh);
    void clearMeshes();
    size_t countMeshes();

    bool operator==(MaterialRuntime& other);

    void debug();
    void logMeshes();

    void drawShadowPass(ShaderRuntime& shader);

    bool loadFromDefinition() override;

    optional<reference_wrapper<TextureRuntime>> getAlbedoTexture() const;
    void setAlbedoTexture(TextureRuntime& texture);

    optional<reference_wrapper<TextureRuntime>> getNormalTexture() const;
    void setNormalTexture(TextureRuntime& texture);

    optional<reference_wrapper<TextureRuntime>> getMetallicTexture() const;
    void setMetallicTexture(TextureRuntime& texture);

    optional<reference_wrapper<TextureRuntime>> getRoughnessTexture() const;
    void setRoughnessTexture(TextureRuntime& texture);

    optional<reference_wrapper<TextureRuntime>> getAoTexture() const;
    void setAoTexture(TextureRuntime& texture);

    optional<reference_wrapper<ShaderRuntime>> getShader() const;
    void setShader(ShaderRuntime& shader);

    void pushTasks() override;

    // Used because InstanceVector is of type Entity*
    vector<reference_wrapper<ModelMesh>> getUsedByVector() const;

  protected:
    optional<reference_wrapper<TextureRuntime>> mAlbedoTexture;
    optional<reference_wrapper<TextureRuntime>> mNormalTexture;
    optional<reference_wrapper<TextureRuntime>> mMetallicTexture;
    optional<reference_wrapper<TextureRuntime>> mRoughnessTexture;
    optional<reference_wrapper<TextureRuntime>> mAoTexture;
    optional<reference_wrapper<ShaderRuntime>>  mShader;
    // Used as InstanceVector is Entity*
    vector<reference_wrapper<ModelMesh>> mUsedBy;
  };
}

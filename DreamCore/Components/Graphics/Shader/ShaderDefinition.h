#pragma once

#include "Components/AssetDefinition.h"

namespace octronic::dream
{
  class ShaderDefinition : public AssetDefinition
  {
  public:
    ShaderDefinition(ProjectDefinition& pd, const json& js);
    ShaderDefinition(ShaderDefinition&&) = default;
    ShaderDefinition& operator=(ShaderDefinition&&) = default;
  };
}

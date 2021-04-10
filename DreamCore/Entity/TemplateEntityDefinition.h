#pragma once

#include <optional>
#include <iostream>
#include <vector>
#include <memory>

#include "Common/AssetType.h"
#include "Base/Definition.h"
#include "Math/Transform.h"

using glm::vec4;
using std::map;
using std::optional;
using std::reference_wrapper;
using std::vector;

namespace octronic::dream
{
  class AssetDefinition;
  class ProjectDefinition;

  class TemplateEntityDefinition : public Definition
  {
  public:
    TemplateEntityDefinition(ProjectDefinition& sceneDefinition, const json& data);

    TemplateEntityDefinition(TemplateEntityDefinition&&) = default;
    TemplateEntityDefinition& operator=(TemplateEntityDefinition&&) = default;

    ProjectDefinition& getProjectDefinition();
    json getJson() override;

    TemplateEntityDefinition& duplicateDefinition();

    int getSelectedAssetDefinitionIndex(AssetType) const;
    void setSelectedAssetDefinitionIndex(AssetType, unsigned long index);
    UuidType getAssetDefinition(AssetType) const;
    void setAssetDefinition(AssetType, UuidType uuid);
    map<AssetType, UuidType> getAssetDefinitionsMap() const;

  private:
    reference_wrapper<ProjectDefinition> mProjectDefinition;
  };
}

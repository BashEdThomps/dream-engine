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
    json toJson() override;

    TemplateEntityDefinition& duplicateDefinition();

    unsigned long getSelectedAssetDefinitionIndex(AssetType) const;
    void setSelectedAssetDefinitionIndex(AssetType, unsigned long index);
    UuidType getAssetDefinition(AssetType) const;
    void setAssetDefinition(AssetType, UuidType uuid);
    map<AssetType, UuidType> getAssetDefinitionsMap() const;

  private:
    reference_wrapper<ProjectDefinition> mProjectDefinition;
  };
}

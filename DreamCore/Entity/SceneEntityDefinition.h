#pragma once

#include "Base/Definition.h"

#include <optional>
#include <vector>
#include <glm/vec4.hpp>

using std::reference_wrapper;
using std::optional;
using std::vector;
using glm::vec4;

namespace octronic::dream
{
  class SceneDefinition;
  class Transform;

  class SceneEntityDefinition : public Definition
  {
  public:
    SceneEntityDefinition(
        SceneDefinition& scene,
        optional<reference_wrapper<SceneEntityDefinition>> parent,
        const json& js);

    Transform getTransform() const;
    void setTransform(const Transform&);

    UuidType getTemplateUuid() const;
    void setTemplateUuid(UuidType);

    SceneEntityDefinition& createChildDefinition();
    SceneEntityDefinition& createChildDefinitionFrom(SceneEntityDefinition&);
    void removeChildDefinition(SceneEntityDefinition&);
    vector<SceneEntityDefinition>& getChildDefinitionsVector();
    size_t countChildDefinitions() const;
    SceneEntityDefinition& duplicateDefinition();
    optional<reference_wrapper<SceneEntityDefinition>> getChildDefinitionByUuid(UuidType);

    SceneDefinition& getSceneDefinition();

    void setParentDefinition(SceneEntityDefinition&);
    optional<reference_wrapper<SceneEntityDefinition>>& getParentDefinition();
    bool hasParentDefinition() const;

    void setFontColor(const vec4& color);
    vec4 getFontColor() const;

    void setFontText(const string& text);
    string getFontText() const;

    void setFontScale(float s);
    float getFontScale() const;

  private:
    reference_wrapper<SceneDefinition> mSceneDefinition;
    optional<reference_wrapper<SceneEntityDefinition>> mParentDefinition;
    vector<SceneEntityDefinition> mChildDefinitions;
  };
}

#pragma once

#include "PropertyType.h"

#include <DreamCore.h>

#include <optional>
#include <memory>

using std::optional;
using std::reference_wrapper;
using octronic::dream::Definition;
using octronic::dream::Runtime;

namespace octronic::dream::tool
{
  class PropertyTarget
  {
  public:
    PropertyTarget();
    PropertyTarget(PropertyType, optional<reference_wrapper<Definition>>);

    PropertyType getType() const;
    void setType(PropertyType type);

    optional<reference_wrapper<Definition>> getDefinition() const;
    void setDefinition(optional<reference_wrapper<Definition>> definition);


  private:
    PropertyType mType;
    optional<reference_wrapper<Definition>> mDefinition;
  };
}


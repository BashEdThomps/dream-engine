#include "PropertyTarget.h"

namespace octronic::dream::tool
{
  PropertyTarget::PropertyTarget
  ()
  {

  }


  PropertyTarget::PropertyTarget
  (PropertyType type,
   optional<reference_wrapper<Definition>> def)
    : mType(type),
      mDefinition(def)
  {
  }

  PropertyType
  PropertyTarget::getType
  ()
  const
  {
    return mType;
  }

  void
  PropertyTarget::setType
  (PropertyType type)
  {
    mType = type;
  }

  void
  PropertyTarget::setDefinition
  (optional<reference_wrapper<Definition>> definition)
  {
    mDefinition = definition;
  }

  optional<reference_wrapper<Definition>>
  PropertyTarget::getDefinition
  ()
  const
  {
    return mDefinition;
  }
}

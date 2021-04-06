#pragma once

#include "Common/Uuid.h"
#include "Common/Constants.h"
#include "Math/Transform.h"

#include <json.hpp>

#include <memory>

using nlohmann::json;
using std::reference_wrapper;

namespace octronic::dream
{
  class PhysicsObjectDefinition;

  class CompoundChildDefinition
  {
  public:
    reference_wrapper<PhysicsObjectDefinition> parent;
    Transform transform;
    UuidType uuid;

  public:
    CompoundChildDefinition(PhysicsObjectDefinition& p, const Transform& tx, UuidType id)
      : parent(p),
        transform(tx),
        uuid(id)
    {

    }

    json getJson() const
    {
      json retval = json::object();
      retval[Constants::UUID] = uuid;
      retval[Constants::TRANSFORM] = transform.toJson();
      return retval;
    }
  };
}

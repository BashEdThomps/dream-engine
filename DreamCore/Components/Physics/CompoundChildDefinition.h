#pragma once

#include "Common/Uuid.h"
#include "Common/Constants.h"
#include "Math/Transform.h"

#include <json.hpp>

#include <memory>

using std::shared_ptr;
using nlohmann::json;

namespace octronic::dream
{
	class PhysicsObjectDefinition;

	class CompoundChildDefinition
    {
    public:
       shared_ptr<PhysicsObjectDefinition> parent;
       Transform transform;
       UuidType uuid;

       json getJson() const
       {
           json retval = json::object();
           retval[Constants::UUID] = uuid;
           retval[Constants::TRANSFORM] = transform.toJson();
           return retval;
       }
    };
}

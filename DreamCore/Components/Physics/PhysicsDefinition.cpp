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
#include "PhysicsDefinition.h"
#include "Math/Vector.h"

using std::static_pointer_cast;

namespace octronic::dream
{
  PhysicsDefinition::PhysicsDefinition
  (ProjectDefinition& pd,const json &js)
    : AssetDefinition(pd,js)
  {

  }

  void
  PhysicsDefinition::setMass
  (float mass)
  {
    mJson[Constants::ASSET_ATTR_MASS] = mass;
  }

  float
  PhysicsDefinition::getMass
  ()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_MASS) == mJson.end())
    {
      return 0;
    }
    return mJson[Constants::ASSET_ATTR_MASS];
  }

  void
  PhysicsDefinition::setMargin
  (float margin)
  {
    mJson[Constants::ASSET_ATTR_MARGIN] = margin;
  }

  float
  PhysicsDefinition::getMargin
  ()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_MARGIN) == mJson.end())
    {
      return 0.f;
    }
    return mJson[Constants::ASSET_ATTR_MARGIN];
  }

  void
  PhysicsDefinition::setKinematic
  (bool kinematic)
  {
    mJson[Constants::ASSET_ATTR_KINEMATIC] = kinematic;
  }

  bool
  PhysicsDefinition::getKinematic
  ()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_KINEMATIC) == mJson.end())
    {
      return false;
    }
    return mJson[Constants::ASSET_ATTR_KINEMATIC];
  }

  void
  PhysicsDefinition::setHalfExtents
  (const vec3& halfExtent)
  {
    mJson[Constants::ASSET_ATTR_SIZE] = Vector3::toJson(halfExtent);
  }

  vec3
  PhysicsDefinition::getHalfExtents
  ()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_SIZE) == mJson.end())
    {
      return vec3(0.f);
    }
    return Vector3::fromJson(mJson[Constants::ASSET_ATTR_SIZE]);
  }

  void
  PhysicsDefinition::setNormal
  (vec3 n)
  {
    mJson[Constants::ASSET_ATTR_NORMAL] = Vector3::toJson(n);
  }

  vec3
  PhysicsDefinition::getNormal
  ()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_NORMAL) == mJson.end())
    {
      return vec3(0.f);
    }
    return Vector3::fromJson(mJson[Constants::ASSET_ATTR_NORMAL]);
  }

  float
  PhysicsDefinition::getRadius
  ()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_RADIUS) == mJson.end())
    {
      return 0;
    }
    return mJson[Constants::ASSET_ATTR_RADIUS];
  }

  void
  PhysicsDefinition::setRadius
  (float rad)
  {
    mJson[Constants::ASSET_ATTR_RADIUS] = rad;
  }

  float
  PhysicsDefinition::getHeight
  ()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_HEIGHT) == mJson.end())
    {
      return 0;
    }
    return mJson[Constants::ASSET_ATTR_HEIGHT];
  }

  void PhysicsDefinition::setHeight(float height)
  {
    mJson[Constants::ASSET_ATTR_HEIGHT] = height;
  }

  float
  PhysicsDefinition::getConstant
  ()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_CONSTANT) == mJson.end())
    {
      return 0;
    }
    return mJson[Constants::ASSET_ATTR_CONSTANT];
  }

  void
  PhysicsDefinition::setConstant
  (float constant)
  {
    mJson[Constants::ASSET_ATTR_CONSTANT] = constant;
  }

  bool
  PhysicsDefinition::getControllableCharacter
  ()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_CONTROLLABLE) == mJson.end())
    {
      return false;
    }
    return mJson[Constants::ASSET_ATTR_CONTROLLABLE];
  }

  void
  PhysicsDefinition::setControllableCharacter
  (bool controllable)
  {
    mJson[Constants::ASSET_ATTR_CONTROLLABLE] = controllable;
  }

  void PhysicsDefinition::setCcdSweptSphereRadius(float rad)
  {
    mJson[Constants::ASSET_ATTR_CCD_SPR] = rad;
  }

  float
  PhysicsDefinition::getCcdSweptSphereRadius
  ()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_CCD_SPR) == mJson.end())
    {
      return 0.f;
    }
    return mJson[Constants::ASSET_ATTR_CCD_SPR];
  }

  float
  PhysicsDefinition::getRestitution
  ()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_RESTITUTION) == mJson.end())
    {
      return 0.f;
    }
    return mJson[Constants::ASSET_ATTR_RESTITUTION];
  }

  void
  PhysicsDefinition::setRestitution
  (float r)
  {
    mJson[Constants::ASSET_ATTR_RESTITUTION] = r;
  }

  float
  PhysicsDefinition::getFriction
  ()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_FRICTION) == mJson.end())
    {
      return 0.f;
    }
    return mJson[Constants::ASSET_ATTR_FRICTION];
  }

  void
  PhysicsDefinition::setFriction
  (float r)
  {
    mJson[Constants::ASSET_ATTR_FRICTION] = r;
  }


  void
  PhysicsDefinition::addCompoundChild
  (const CompoundChildDefinition& def)
  {
    if (mJson.find(Constants::ASSET_ATTR_COMPOUND_CHILDREN) == mJson.end())
    {
      mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN] = json::array();
    }
    mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN].push_back(def.getJson());
  }

  vector<CompoundChildDefinition>
  PhysicsDefinition::getCompoundChildren
  ()
  {
    vector<CompoundChildDefinition> retval;
    if (mJson.find(Constants::ASSET_ATTR_COMPOUND_CHILDREN) == mJson.end())
    {
      return retval;
    }

    for (json childJson : mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN])
    {
      retval.emplace_back(*this, Transform(childJson[Constants::TRANSFORM]), childJson[Constants::UUID]);
    }
    return retval;
  }

  void
  PhysicsDefinition::updateCompoundChildTransform
  (const CompoundChildDefinition& def)
  {
    if (mJson.find(Constants::ASSET_ATTR_COMPOUND_CHILDREN) == mJson.end())
    {
      return;
    }

    for(auto iter = begin(mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN]);
        iter != end(mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN]); iter++)
    {
      UuidType jsUuid = (*iter)[Constants::UUID];
      if (def.uuid == jsUuid)
      {
        (*iter)[Constants::TRANSFORM] = def.transform.toJson();
        return;
      }
    }
  }

  void
  PhysicsDefinition::removeCompoundChild
  (const CompoundChildDefinition& def)
  {
    if (mJson.find(Constants::ASSET_ATTR_COMPOUND_CHILDREN) == mJson.end())
    {
      return;
    }

    for(auto iter = begin(mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN]);
        iter != end(mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN]); iter++ )
    {
      if (*iter == def.getJson())
      {
        mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN].erase(iter);
        return;
      }
    }
  }

  void
  PhysicsDefinition::makeCompoundChildren
  ()
  {
    if (mJson.find(Constants::ASSET_ATTR_COMPOUND_CHILDREN) == mJson.end())
    {
      mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN] = json::array();
    }
  }

  UuidType
  PhysicsDefinition::getCollisionModelUuid
  ()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_COLLISION_MODEL) == mJson.end())
    {
      return Uuid::INVALID;
    }
    return mJson[Constants::ASSET_ATTR_COLLISION_MODEL];
  }

  void
  PhysicsDefinition::setCollisionModelUuid
  (UuidType modelUuid)
  {
    mJson[Constants::ASSET_ATTR_COLLISION_MODEL] = modelUuid;
  }

  vec3
  PhysicsDefinition::getLinearFactor
  ()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_LINEAR_FENTITY) == mJson.end())
    {
      return vec3(0.0f);
    }
    return vec3(mJson[Constants::ASSET_ATTR_LINEAR_FENTITY]);
  }

  void
  PhysicsDefinition::setLinearFactor
  (const vec3& lf)
  {
    mJson[Constants::ASSET_ATTR_LINEAR_FENTITY] = Vector3::toJson(lf);
  }

  vec3
  PhysicsDefinition::getAngularFactor
  ()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_ANGULAR_FENTITY) == mJson.end())
    {
      return vec3(0.f);
    }
    return vec3(mJson[Constants::ASSET_ATTR_ANGULAR_FENTITY]);
  }

  void
  PhysicsDefinition::setAngularFactor
  (const vec3& af)
  {
    mJson[Constants::ASSET_ATTR_ANGULAR_FENTITY] = Vector3::toJson(af);
  }

  vec3
  PhysicsDefinition::getLinearVelocity
  ()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_LINEAR_VELOCITY) == mJson.end())
    {
      return vec3(0.f);
    }
    return vec3(mJson[Constants::ASSET_ATTR_LINEAR_VELOCITY]);
  }

  void
  PhysicsDefinition::setLinearVelocity
  (const vec3& lf)
  {
    mJson[Constants::ASSET_ATTR_LINEAR_VELOCITY] = Vector3::toJson(lf);
  }

  vec3
  PhysicsDefinition::getAngularVelocity
  ()
  const
  {
    if (mJson.find(Constants::ASSET_ATTR_ANGULAR_VELOCITY) == mJson.end())
    {
      return vec3(0.f);
    }
    return vec3(mJson[Constants::ASSET_ATTR_ANGULAR_VELOCITY]);
  }

  void
  PhysicsDefinition::setAngularVelocity
  (const vec3& af)
  {
    mJson[Constants::ASSET_ATTR_ANGULAR_VELOCITY] = Vector3::toJson(af);
  }
}

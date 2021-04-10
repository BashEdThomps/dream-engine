#include "PhysicsMotionState.h"

#include "Common/Logger.h"
#include "Entity/EntityRuntime.h"

#include <iostream>
#include <LinearMath/btMatrix3x3.h>
#include <LinearMath/btQuaternion.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace octronic::dream
{
  PhysicsMotionState::PhysicsMotionState
  (EntityRuntime& entity)
    : btMotionState(),
      mEntityRuntime(entity)
  {
    LOG_TRACE( "PhysicsMotionState: Constructor called" );
  }

  PhysicsMotionState::~PhysicsMotionState
  ()
  {
    LOG_TRACE( "PhysicsMotionState: Destroying Object" );
  }

  void
  PhysicsMotionState::getWorldTransform
  (btTransform& worldTrans) const
  {
    const auto& tx = getEntityRuntime().getTransform();
    worldTrans.setFromOpenGLMatrix(glm::value_ptr(tx.getMatrix()));
  }

  void
  PhysicsMotionState::setWorldTransform
  (const btTransform& worldTrans)
  {
    LOG_DEBUG( "PhysicsMotionState: setWorldTransform called" );
    auto origin = worldTrans.getOrigin();
    float yaw;
    float pitch;
    float roll;
    worldTrans.getRotation().getEulerZYX(yaw, pitch, roll);
    Transform tx;
    tx.setTranslation(Vector3::fromBullet(origin));
    tx.setYaw(yaw);
    tx.setPitch(pitch);
    tx.setRoll(roll);
    getEntityRuntime().setTransform(tx);
  }

  void
  PhysicsMotionState::setKinematicPos
  (btTransform &trans)
  {
    LOG_DEBUG( "PhysicsMotionState: setKinematicPos called" );
    setWorldTransform(trans);
  }


  EntityRuntime&
  PhysicsMotionState::getEntityRuntime
  ()
  const
  {
    return mEntityRuntime.get();
  }
}

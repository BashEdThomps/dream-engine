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
  (const weak_ptr<EntityRuntime>& entity)
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
    if (auto erLock = mEntityRuntime.lock())
    {
      auto tx = erLock->getTransform();
      worldTrans.setFromOpenGLMatrix(glm::value_ptr(tx.getMatrix()));
    }
  }

  void
  PhysicsMotionState::setWorldTransform
  (const btTransform& worldTrans)
  {
    LOG_DEBUG( "PhysicsMotionState: setWorldTransform called" );
    if (auto erLock = mEntityRuntime.lock())
    {
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
      erLock->setTransform(tx);
    }
  }

  void
  PhysicsMotionState::setKinematicPos
  (btTransform &trans)
  {
    LOG_DEBUG( "PhysicsMotionState: setKinematicPos called" );
    setWorldTransform(trans);
  }
}

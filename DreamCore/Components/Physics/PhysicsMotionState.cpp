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

#include <iostream>
#include <LinearMath/btMatrix3x3.h>
#include <LinearMath/btQuaternion.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace octronic::dream
{
    PhysicsMotionState::PhysicsMotionState
    (Transform* dreamTransform)
        : btMotionState(),
          mDreamTransform(dreamTransform)
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
    (btTransform &worldTrans) const
    {
        float* mtx = mDreamTransform->getMatrixFloatPointer();
        worldTrans.setFromOpenGLMatrix(mtx);
    }

    void
    PhysicsMotionState::setWorldTransform
    (const btTransform &worldTrans)
    {

        LOG_DEBUG( "PhysicsMotionState: setWorldTransform called" );
        float mtx[16];
        worldTrans.getOpenGLMatrix(mtx);
        mDreamTransform->setMatrix(glm::make_mat4(mtx));
    }

    void
    PhysicsMotionState::setKinematicPos
    (btTransform &trans)
    {
        LOG_DEBUG( "PhysicsMotionState: setKinematicPos called" );
        setWorldTransform(trans);
    }
}

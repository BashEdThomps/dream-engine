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

#include <iostream>

#include <LinearMath/btMatrix3x3.h>
#include <LinearMath/btQuaternion.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Dream
{
    PhysicsMotionState::PhysicsMotionState
    (Transform& dreamTransform)
        : btMotionState(),
          DreamObject("PhysicsMotionState"),
          mDreamTransform(dreamTransform)
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->trace( "Constructor called" );
        #endif
    }

    PhysicsMotionState::~PhysicsMotionState
    ()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->trace( "Destroying Object" );
        #endif
    }

    void
    PhysicsMotionState::setTransform
    (const Transform& transform)
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug( "setTransform called" );
        #endif
        mDreamTransform.setMatrix(transform.getMatrix());
    }

    void
    PhysicsMotionState::getWorldTransform
    (btTransform &worldTrans) const
    {
        auto mtx = mDreamTransform.getMatrix();
        float mtxPtr[16];
        memcpy(&mtxPtr[0],glm::value_ptr(mtx),sizeof(float)*16);
        worldTrans.setFromOpenGLMatrix(mtxPtr);
    }

    void
    PhysicsMotionState::setWorldTransform
    (const btTransform &worldTrans)
    {

        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug( "setWorldTransform called" );
        #endif
        float mtx[16];
        worldTrans.getOpenGLMatrix(mtx);
        mDreamTransform.setMatrix(glm::make_mat4(mtx));
    }

    void
    PhysicsMotionState::setKinematicPos
    (btTransform &trans)
    {

        #ifdef DREAM_LOG
        auto log = getLog();
        log->debug( "setKinematicPos called" );
        #endif
        setWorldTransform(trans);
    }
} // End of Dream

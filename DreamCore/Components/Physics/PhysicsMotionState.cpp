/*
 * PhysicsMotionState.cpp
 *
 * Created: 25/12/2016 2016 by Ashley
 *
 * Copyright 2016 Octronic. All rights reserved.
 *
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

#include "../../Common/Constants.h"

using std::cout;
using std::cerr;

namespace Dream
{
    PhysicsMotionState::PhysicsMotionState
    (Transform3D* dreamTransform)
        : btMotionState(),
          DreamObject("PhysicsMotionState"),
          mDreamTransform(dreamTransform)
    {
        auto log = getLog();
        log->trace( "Constructor called" );
    }

    PhysicsMotionState::~PhysicsMotionState
    ()
    {
        auto log = getLog();
        log->trace( "Destroying Object" );
    }

    void
    PhysicsMotionState::setTransform
    (Transform3D* transform)
    {
        auto log = getLog();
        log->debug( "setTransform called" );
        mDreamTransform = transform;
    }

    void
    PhysicsMotionState::getWorldTransform
    (btTransform &worldTrans) const
    {
        auto log = getLog();
        log->debug( "getWorldTransform called" );
        worldTrans.setOrigin(mDreamTransform->getTranslationAsBtVector3());
        worldTrans.setRotation(mDreamTransform->getOrientationAsBtQuaternion());
    }

    void
    PhysicsMotionState::setWorldTransform
    (const btTransform &worldTrans)
    {
        auto log = getLog();
        log->debug( "setWorldTransform called" );

        // Translation
        btVector3 pos = worldTrans.getOrigin();
        mDreamTransform->setTranslation(pos.x(), pos.y(), pos.z());
        // Rotation
        btQuaternion rot = worldTrans.getRotation();
        mDreamTransform->setOrientation(rot.getW(),rot.getX(),rot.getY(),rot.getZ());
    }

    void
    PhysicsMotionState::setKinematicPos
    (btTransform &trans)
    {
        auto log = getLog();
        log->debug( "setKinematicPos called" );
        btVector3 pos = trans.getOrigin();
        mDreamTransform->setTranslation(pos.x(), pos.y(), pos.z());
    }


} // End of Dream

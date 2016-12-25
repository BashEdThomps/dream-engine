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

namespace Dream {
    PhysicsMotionState::PhysicsMotionState(Transform3D* dreamTransform) : btMotionState() {
      mDreamTransform = dreamTransform;
    }

    PhysicsMotionState::~PhysicsMotionState() {}

    void PhysicsMotionState::setTransform(Transform3D* transform) {
      mDreamTransform = transform;
    }

    void PhysicsMotionState::getWorldTransform(btTransform &worldTrans) const {
      worldTrans.setOrigin(mDreamTransform->getTranslationAsBtVector3());
      btQuaternion rot = worldTrans.getRotation();
      rot.setEulerZYX(
        mDreamTransform->getRotationX(),
        mDreamTransform->getRotationY(),
        mDreamTransform->getRotationZ()
      );
      worldTrans.setRotation(rot);
    }

    void PhysicsMotionState::setWorldTransform(const btTransform &worldTrans) {
      if(mDreamTransform == nullptr) {
        return; // die quietly before we set a node
      }
      btQuaternion rot = worldTrans.getRotation();
      mDreamTransform->setRotation(rot.getX(),rot.getY(),rot.getZ());
      btVector3 pos = worldTrans.getOrigin();
      mDreamTransform->setTranslation(pos.x(), pos.y(), pos.z());
    }

} // End of Dream

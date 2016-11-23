/*
 * Transform3D.cpp
 *
 * Created: 22/11/2016 2016 by Ashley
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
#include "Transform3D.h"

namespace Dream {

    Transform3D::Transform3D() {
      mTranslation = std::vector<float>(3);
      mRotation = std::vector<float>(3);
      mScale = std::vector<float>(3);
    }

    std::vector<float> Transform3D::getTranslation() {
      return mTranslation;
    }

    void Transform3D::setTranslation(std::vector<float> translation) {
      mTranslation = translation;
    }

    void Transform3D::setTranslation(float x, float y, float z) {
      mTranslation[TRANSFORM_X] = x;
      mTranslation[TRANSFORM_Y] = y;
      mTranslation[TRANSFORM_Z] = z;
    }

    std::vector<float> Transform3D::getRotation() {
      return mRotation;
    }

    void Transform3D::setRotation(std::vector<float> rotation) {
      mRotation = rotation;
    }

    void Transform3D::setRotation(float x, float y, float z) {
      mRotation[TRANSFORM_X] = x;
      mRotation[TRANSFORM_Y] = y;
      mRotation[TRANSFORM_Z] = z;
    }

    std::vector<float> Transform3D::getScale() {
      return mScale;
    }

    void Transform3D::setScale(std::vector<float> scale) {
        mScale = scale;
    }

    void Transform3D::setScale(float x, float y, float z) {
        mScale[TRANSFORM_X] = x;
        mScale[TRANSFORM_Y] = y;
        mScale[TRANSFORM_Z] = z;
    }
}

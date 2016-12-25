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
      mTransformType = TRANSFORM_TYPE_ABSOLUTE;
      mTranslation = vector<float>(3);
      mRotation = vector<float>(3);
      mScale = vector<float>(3);
    }

    Transform3D::Transform3D(Transform3D* obj) {
      mTransformType = obj->getTransformType();
      mTranslation = vector<float>(obj->getTranslation());
      mRotation = vector<float>(obj->getRotation());
      mScale = vector<float>(obj->getScale());
    }

// Translation ===================================================================

    vector<float> Transform3D::getTranslation() {
      return mTranslation;
    }

    void Transform3D::setTranslation(vector<float> translation) {
      mTranslation = translation;
    }

    void Transform3D::setTranslation(float x, float y, float z) {
      mTranslation[TRANSFORM_X] = x;
      mTranslation[TRANSFORM_Y] = y;
      mTranslation[TRANSFORM_Z] = z;
    }

    void Transform3D::setTranslationX(float x) {
      mTranslation[TRANSFORM_X] = x;
    }

    void Transform3D::setTranslationY(float y) {
      mTranslation[TRANSFORM_Y] = y;
    }

    void Transform3D::setTranslationZ(float z) {
      mTranslation[TRANSFORM_Z] = z;
    }

    float Transform3D::getTranslationX() {
      return mTranslation[TRANSFORM_X];
    }

    float Transform3D::getTranslationY() {
      return mTranslation[TRANSFORM_Y];
    }

    float Transform3D::getTranslationZ() {
      return mTranslation[TRANSFORM_Z];
    }

// Rotation ======================================================================

    vector<float> Transform3D::getRotation() {
      return mRotation;
    }

    void Transform3D::setRotation(vector<float> rotation) {
      mRotation = rotation;
    }

    void Transform3D::setRotation(float x, float y, float z) {
      mRotation[TRANSFORM_X] = x;
      mRotation[TRANSFORM_Y] = y;
      mRotation[TRANSFORM_Z] = z;
    }

    float Transform3D::getRotationX() {
      return mRotation[TRANSFORM_X];
    }

    float Transform3D::getRotationY() {
      return mRotation[TRANSFORM_Y];
    }

    float Transform3D::getRotationZ() {
      return mRotation[TRANSFORM_Z];
    }

    void Transform3D::setRotationX(float x) {
      mRotation[TRANSFORM_X] = x;
    }

    void Transform3D::setRotationY(float y) {
      mRotation[TRANSFORM_Y] = y;
    }

    void Transform3D::setRotationZ(float z) {
      mRotation[TRANSFORM_Z] = z;
    }

// Scale =========================================================================

    vector<float> Transform3D::getScale() {
      return mScale;
    }

    void Transform3D::setScale(vector<float> scale) {
        mScale = scale;
    }

    void Transform3D::setScale(float x, float y, float z) {
        mScale[TRANSFORM_X] = x;
        mScale[TRANSFORM_Y] = y;
        mScale[TRANSFORM_Z] = z;
    }

    float Transform3D::getScaleX() {
      return mScale[TRANSFORM_X];
    }

    float Transform3D::getScaleY() {
      return mScale[TRANSFORM_Y];
    }

    float Transform3D::getScaleZ() {
      return mScale[TRANSFORM_Z];
    }

    void Transform3D::setScaleX(float x) {
      mScale[TRANSFORM_X] = x;
    }

    void Transform3D::setScaleY(float y) {
      mScale[TRANSFORM_Y] = y;
    }

    void Transform3D::setScaleZ(float z) {
      mScale[TRANSFORM_Z] = z;
    }

    void Transform3D::translateByX(float delta) {
      mTranslation[TRANSFORM_X] += delta;
    }

    void Transform3D::translateByY(float delta) {
      mTranslation[TRANSFORM_Y] += delta;
    }

    void Transform3D::translateByZ(float delta) {
      mTranslation[TRANSFORM_Z] += delta;
    }

    void Transform3D::rotateByX(float delta) {
      mRotation[TRANSFORM_X] += delta;
    }

    void Transform3D::rotateByY(float delta) {
      mRotation[TRANSFORM_Y] += delta;
    }

    void Transform3D::rotateByZ(float delta) {
      mRotation[TRANSFORM_Z] += delta;
    }

    void Transform3D::scaleByX(float delta) {
      mScale[TRANSFORM_X] += delta;
    }

    void Transform3D::scaleByY(float delta) {
      mScale[TRANSFORM_Y] += delta;
    }

    void Transform3D::scaleByZ(float delta) {
      mScale[TRANSFORM_Z] += delta;
    }

// Transform Type ================================================================

    void Transform3D::setTransformType(string type) {
      mTransformType = type;
    }

    string Transform3D::getTransformType() {
      return mTransformType;
    }

    btVector3 Transform3D::getTranslationAsBtVector3() {
      return btVector3(getTranslationX(),getTranslationY(),getTranslationZ());
    }

    btVector3 Transform3D::getRotationAsBtVector3() {
      return btVector3(getRotationX(),getRotationY(),getRotationZ());
    }
} // End of Dream

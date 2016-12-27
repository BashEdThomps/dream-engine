/*
 * Transform3D.h
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
#ifndef TRANSFORM3D_H
#define TRANSFORM3D_H

#include <vector>
#include <iostream>
#include <LinearMath/btVector3.h>
#include <glm/gtc/quaternion.hpp>
#include "Constants.h"

namespace Dream {

  using namespace std;

  class Transform3D {
  private:
    vector<float> mTranslation;
    //vector<float> mRotation;
    glm::quat mOrientation;
    vector<float> mScale;
    string mTransformType;
  public:
    Transform3D();
    Transform3D(Transform3D*);
    vector<float> getTranslation();
    void setTranslation(vector<float>);
    void setTranslation(float, float, float);
    float getTranslationX();
    float getTranslationY();
    float getTranslationZ();
    void setTranslationX(float);
    void setTranslationY(float);
    void setTranslationZ(float);
    btVector3 getTranslationAsBtVector3();

    glm::quat getOrientation();
    void setOrientation(glm::quat);

    vector<float> getRotation();
    void setRotation(vector<float>);
    void setRotation(float, float, float);
    float getRotationX();
    float getRotationY();
    float getRotationZ();
    void setRotationX(float);
    void setRotationY(float);
    void setRotationZ(float);
    btVector3 getRotationAsBtVector3();

    vector<float> getScale();
    void setScale(vector<float>);
    void setScale(float, float, float);
    float getScaleX();
    float getScaleY();
    float getScaleZ();
    void setScaleX(float);
    void setScaleY(float);
    void setScaleZ(float);

    string getTransformType();
    void setTransformType(string);

    void translateByX(float);
    void translateByY(float);
    void translateByZ(float);

    void rotateByX(float);
    void rotateByY(float);
    void rotateByZ(float);

    void scaleByX(float);
    void scaleByY(float);
    void scaleByZ(float);
  };

} // End of Dream

#endif // TRANSFORM3D_H

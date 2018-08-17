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

#pragma once

#pragma once

#ifdef __APPLE__
    #include <nlohmann/json.hpp>
#else
    #include <json.hpp>
#endif


#include <string>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>
#include "../Common/ILoggable.h"

using std::string;
using nlohmann::json;
using glm::vec3;
using glm::quat;

namespace Dream
{
  class Transform3D : public ILoggable
  {
  private:
    vec3 mTranslation;
    quat mOrientation;
    vec3 mScale;
    string mTransformType;
    Transform3D* mParentHandle;

  public:
    Transform3D();
    explicit Transform3D(json j);
    vec3 getTranslation();
    void setTranslation(vec3);
    void setTranslation(float, float, float);
    float getTranslationX();
    float getTranslationY();
    float getTranslationZ();
    void setTranslationX(float);
    void setTranslationY(float);
    void setTranslationZ(float);
    btVector3 getTranslationAsBtVector3();
    btQuaternion getOrientationAsBtQuaternion();
    btTransform getTransformAsBtTransform();

    quat getOrientation();
    void setOrientation(float,float,float,float);
    void setOrientation(quat);

    vec3 getRotation();
    void setRotation(float, float, float);
    void setRotation(vec3 rot);
    float getRotationX();
    float getRotationY();
    float getRotationZ();
    void setRotationX(float);
    void setRotationY(float);
    void setRotationZ(float);
    btVector3 getRotationAsBtVector3();

    vec3 getScale();
    void setScale(vec3);
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

    json getJson();
  };

} // End of Dream

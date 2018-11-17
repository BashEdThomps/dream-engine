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

#include "../deps/json/json.hpp"
#include <string>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>
#include "../Common/DreamObject.h"

using namespace std;
using namespace nlohmann;
using namespace glm;

namespace Dream
{

    enum TransformType
    {
        Absolute,
        Offset
    };

  class Transform3D : public DreamObject
  {
  private:
    vec3 mTranslation;
    quat mOrientation;
    vec3 mScale;
    TransformType mTransformType;

  public:
    Transform3D();
    Transform3D(glm::mat4 fromMatrix);
    explicit Transform3D(json j);
    ~Transform3D() override;

    bool isTypeOffset() const;
    bool isTypeAbsolute() const;

    vec3 getTranslation() const;
    void setTranslation(vec3);
    void setTranslation(float, float, float);
    float getTranslationX() const;
    float getTranslationY() const;
    float getTranslationZ() const;
    void setTranslationX(float);
    void setTranslationY(float);
    void setTranslationZ(float);
    btVector3 getTranslationAsBtVector3() const;
    btQuaternion getOrientationAsBtQuaternion() const;
    btTransform getTransformAsBtTransform() const;

    quat getOrientation() const;

    float getOrientationW() const;
    void  setOrientationW(float);

    float getOrientationX() const;
    void  setOrientationX(float);

    float getOrientationY() const;
    void setOrientationY(float);

    float getOrientationZ() const;
    void setOrientationZ(float);

    void setOrientation(float,float,float,float);
    void setOrientation(quat);

    vec3 getRotation() const;
    void setRotation(float, float, float);
    void setRotation(vec3 rot);
    float getRotationX() const;
    float getRotationY() const;
    float getRotationZ() const;
    void setRotationX(float);
    void setRotationY(float);
    void setRotationZ(float);
    btVector3 getRotationAsBtVector3() const;

    vec3 getScale() const;
    void setScale(vec3);
    void setScale(float, float, float);
    float getScaleX() const;
    float getScaleY() const;
    float getScaleZ() const;
    void setScaleX(float);
    void setScaleY(float);
    void setScaleZ(float);

    TransformType getTransformType() const;
    void setTransformType(TransformType);

    void translateByX(float);
    void translateByY(float);
    void translateByZ(float);

    void rotateByX(float);
    void rotateByY(float);
    void rotateByZ(float);

    void scaleByX(float);
    void scaleByY(float);
    void scaleByZ(float);

    glm::mat4 asMat4() const;
    void setFromMat4(glm::mat4 mat);

    json getJson();

    void setOffsetFrom(Transform3D*, Transform3D*);
    string transformTypeToString(TransformType t);
    TransformType transformTypeFromString(string);
    void fromMat4(mat4 mtx);
  };

} // End of Dream

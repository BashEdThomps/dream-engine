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

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

namespace Dream
{

    Transform3D::Transform3D
    () : DreamObject("Transform3D")
    {
        mTransformType = Constants::TRANSFORM_TYPE_ABSOLUTE;
        mTranslation   = vec3(0.0f);
        mScale         = vec3(1.0f);
        mOrientation   = quat(1.0f,0.0f,0.0f,0.0f);
    }

    Transform3D::Transform3D(mat4 mtx)
        : DreamObject("Transform3D")
    {
       mTransformType = Constants::TRANSFORM_TYPE_ABSOLUTE;

       setOrientation(quat(mtx));
       setTranslation(glm::vec3(mtx[3]));
       setScale(vec3(1.0f));

    }

    Transform3D::Transform3D
    (json jsonTransform)
    : DreamObject("Transform3D")
    {
        if (!jsonTransform[Constants::TRANSFORM_TYPE].is_null())
        {
            setTransformType(jsonTransform[Constants::TRANSFORM_TYPE]);
        }
        else
        {
            setTransformType(Constants::TRANSFORM_TYPE_ABSOLUTE);
        }

        if (!jsonTransform[Constants::TRANSFORM_TRANSLATION].is_null())
        {
            setTranslation(
                jsonTransform[Constants::TRANSFORM_TRANSLATION][Constants::X],
                jsonTransform[Constants::TRANSFORM_TRANSLATION][Constants::Y],
                jsonTransform[Constants::TRANSFORM_TRANSLATION][Constants::Z]
            );
        }

        if (!jsonTransform[Constants::TRANSFORM_ORIENTATION].is_null())
        {
            setOrientation(
                jsonTransform[Constants::TRANSFORM_ORIENTATION][Constants::W],
                jsonTransform[Constants::TRANSFORM_ORIENTATION][Constants::X],
                jsonTransform[Constants::TRANSFORM_ORIENTATION][Constants::Y],
                jsonTransform[Constants::TRANSFORM_ORIENTATION][Constants::Z]
            );
        }

        if (!jsonTransform[Constants::TRANSFORM_SCALE].is_null())
        {
            setScale(
                jsonTransform[Constants::TRANSFORM_SCALE][Constants::X],
                jsonTransform[Constants::TRANSFORM_SCALE][Constants::Y],
                jsonTransform[Constants::TRANSFORM_SCALE][Constants::Z]
            );
        }
    }

    bool Transform3D::isTypeOffset() const
    {
       return getTransformType().compare(Constants::TRANSFORM_TYPE_OFFSET) == 0;
    }

    bool Transform3D::isTypeAbsolute() const
    {
       return getTransformType().compare(Constants::TRANSFORM_TYPE_ABSOLUTE) == 0;
    }

    // Translation ===================================================================

    vec3
    Transform3D::getTranslation
    ()
    const
    {
        return mTranslation;
    }

    void
    Transform3D::setTranslation
    (vec3 translation)
    {
        mTranslation = translation;
    }

    void
    Transform3D::setTranslation
    (float x, float y, float z)
    {
        mTranslation.x = x;
        mTranslation.y = y;
        mTranslation.z = z;
    }

    void
    Transform3D::setTranslationX
    (float x)
    {
        mTranslation.x = x;
    }

    void
    Transform3D::setTranslationY
    (float y)
    {
        mTranslation.y = y;
    }

    void
    Transform3D::setTranslationZ
    (float z)
    {
        mTranslation.z = z;
    }

    float
    Transform3D::getTranslationX
    ()
    const
    {
        return mTranslation.x;
    }

    float
    Transform3D::getTranslationY
    ()
    const
    {
        return mTranslation.y;
    }

    float
    Transform3D::getTranslationZ
    ()
    const
    {
        return mTranslation.z;
    }

    void
    Transform3D::translateByX
    (float delta)
    {
        mTranslation.x += delta;
    }

    void
    Transform3D::translateByY
    (float delta)
    {
        mTranslation.y += delta;
    }

    void
    Transform3D::translateByZ
    (float delta)
    {
        mTranslation.z += delta;
    }

    // Rotation ======================================================================

    vec3
    Transform3D::getRotation
    ()
    const
    {
        return eulerAngles(mOrientation);
    }

    void
    Transform3D::setRotation
    (vec3 rot)
    {
        mOrientation = quat(rot);
    }

    void
    Transform3D::setRotation
    (float x, float y, float z)
    {
        mOrientation = quat(vec3(x,y,z));
    }

    float
    Transform3D::getRotationX
    ()
    const
    {
        return eulerAngles(mOrientation).x;
    }

    float
    Transform3D::getRotationY
    ()
    const
    {
        return eulerAngles(mOrientation).y;
    }

    float
    Transform3D::getRotationZ
    ()
    const
    {
        return eulerAngles(mOrientation).z;
    }

    void
    Transform3D::setRotationX
    (float x)
    {
        vec3 euler = eulerAngles(mOrientation);
        mOrientation = quat(vec3(x,euler.y,euler.z));
    }

    void
    Transform3D::setRotationY
    (float y)
    {
        vec3 euler = eulerAngles(mOrientation);
        mOrientation = quat(vec3(euler.x,y,euler.z));
    }

    void
    Transform3D::setRotationZ
    (float z)
    {
        vec3 euler = eulerAngles(mOrientation);
        mOrientation = quat(vec3(euler.x,euler.y,z));
    }

    void
    Transform3D::rotateByX
    (float delta)
    {
        mOrientation = quat(vec3(delta,0,0)) * mOrientation;
    }

    void
    Transform3D::rotateByY
    (float delta)
    {
        mOrientation = quat(vec3(0,delta,0)) * mOrientation;
    }

    void
    Transform3D::rotateByZ
    (float delta)
    {
        mOrientation = quat(vec3(0,0,delta)) * mOrientation;
    }

    quat
    Transform3D::getOrientation
    ()
    const
    {
        return mOrientation;
    }

    void
    Transform3D::setOrientation
    (float w, float x, float y, float z)
    {
        mOrientation = quat(w,x,y,z);
    }

    void
    Transform3D::setOrientation
    (quat orientation)
    {
        mOrientation = orientation;
    }

    float Transform3D::getOrientationW() const
    {
       return mOrientation.w;
    }

    void  Transform3D::setOrientationW(float w)
    {
       mOrientation.w = w;
    }

    float Transform3D::getOrientationX() const
    {
       return mOrientation.x;
    }

    void  Transform3D::setOrientationX(float x)
    {
       mOrientation.x = x;
    }

    float Transform3D::getOrientationY() const
    {
        return mOrientation.y;
    }

    void Transform3D::setOrientationY(float y)
    {
        mOrientation.y = y;
    }

    float Transform3D::getOrientationZ() const
    {
       return mOrientation.z;
    }

    void Transform3D::setOrientationZ(float z)
    {
       mOrientation.z = z;
    }

    // Scale ====================================================================

    vec3
    Transform3D::getScale
    ()
    const
    {
        return mScale;
    }

    void
    Transform3D::setScale
    (vec3 scale)
    {
        mScale = scale;
    }

    void
    Transform3D::setScale
    (float x, float y, float z)
    {
        mScale = vec3(x,y,z);
    }

    float
    Transform3D::getScaleX
    ()
    const
    {
        return mScale.x;
    }

    float
    Transform3D::getScaleY
    ()
    const
    {
        return mScale.y;
    }

    float
    Transform3D::getScaleZ
    ()
    const
    {
        return mScale.z;
    }

    void
    Transform3D::setScaleX
    (float x)
    {
        mScale.x = x;
    }

    void
    Transform3D::setScaleY
    (float y)
    {
        mScale.y = y;
    }

    void
    Transform3D::setScaleZ
    (float z)
    {
        mScale.z = z;
    }

    void
    Transform3D::scaleByX
    (float delta)
    {
        mScale.x += delta;
    }

    void
    Transform3D::scaleByY
    (float delta)
    {
        mScale.y += delta;
    }

    void
    Transform3D::scaleByZ
    (float delta)
    {
        mScale.z += delta;
    }

    // Transform Type ===========================================================

    void
    Transform3D::setTransformType
    (string type)
    {
        mTransformType = type;
    }

    string
    Transform3D::getTransformType
    ()
    const
    {
        return mTransformType;
    }

    // Bullet Interfaces ========================================================

    btVector3
    Transform3D::getTranslationAsBtVector3
    ()
    const
    {
        return btVector3(mTranslation.x,mTranslation.y,mTranslation.z);
    }

    btQuaternion
    Transform3D::getOrientationAsBtQuaternion
    ()
    const
    {
        return btQuaternion
        (
            mOrientation.x,
            mOrientation.y,
            mOrientation.z,
            mOrientation.w
        );
    }

    btTransform
    Transform3D::getTransformAsBtTransform
    ()
    const
    {
        btTransform transform;
        transform.setRotation(getOrientationAsBtQuaternion());
        transform.setOrigin(getTranslationAsBtVector3());
        return transform;
    }

    btVector3
    Transform3D::getRotationAsBtVector3
    ()
    const
    {
        return btVector3(getRotationX(),getRotationY(),getRotationZ());
    }

    json
    Transform3D::getJson
    ()
    {
        json j = json::object();
        // Translation Type
        j[Constants::TRANSFORM_TYPE] = getTransformType();
        // Translation
        j[Constants::TRANSFORM_TRANSLATION] = json::object();
        j[Constants::TRANSFORM_TRANSLATION][Constants::X] = getTranslationX();
        j[Constants::TRANSFORM_TRANSLATION][Constants::Y] = getTranslationY();
        j[Constants::TRANSFORM_TRANSLATION][Constants::Z] = getTranslationZ();
        // Orientation
        j[Constants::TRANSFORM_ORIENTATION] = json::object();
        j[Constants::TRANSFORM_ORIENTATION][Constants::W] = mOrientation.w;
        j[Constants::TRANSFORM_ORIENTATION][Constants::X] = mOrientation.x;
        j[Constants::TRANSFORM_ORIENTATION][Constants::Y] = mOrientation.y;
        j[Constants::TRANSFORM_ORIENTATION][Constants::Z] = mOrientation.z;
        // Scale
        j[Constants::TRANSFORM_SCALE] = json::object();
        j[Constants::TRANSFORM_SCALE][Constants::X] = getScaleX();
        j[Constants::TRANSFORM_SCALE][Constants::Y] = getScaleY();
        j[Constants::TRANSFORM_SCALE][Constants::Z] = getScaleZ();
        return j;
    }

    void
    Transform3D::offsetFrom
    (Transform3D* parent, Transform3D* initial)
    {
        auto log = getLog();
        log->trace(
            "Generating offset transform from parent T({},{},{}) R({},{},{})",
            parent->getTranslationX(), parent->getTranslationY(), parent->getTranslationZ(),
            parent->getRotationX(), parent->getRotationY(), parent->getRotationZ()
        );
       // Tx to parent
       mat4 mtx = glm::translate(mat4(1), parent->getTranslation());
       // match parent rotation
       mat4 rot = mat4_cast(parent->getOrientation());
       mtx = mtx*rot;
       // Tx to child (this)
       mtx = glm::translate(mtx, initial->getTranslation());
       // Set child rotation
       rot = mat4_cast(initial->getOrientation());
       mtx = mtx*rot;
       setFromMat4(mtx);
       // Maintain Scale
       setScale(initial->getScale());
    }

    glm::mat4 Transform3D::asMat4() const
    {
       mat4 trans = glm::translate(mat4(1), mTranslation);
       mat4 rot = mat4_cast(mOrientation);
       trans = trans*rot;
       return glm::scale(trans,mScale);
    }

    void Transform3D::setFromMat4(glm::mat4 mtx)
    {
       setOrientation(quat(mtx));
       setTranslation(glm::vec3(mtx[3]));
    }


} // End of Dream

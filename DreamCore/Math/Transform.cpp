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

#define GLM_ENABLE_EXPERIMENTAL

#include "Transform.h"

#include "Math/Vector.h"
#include "Math/Conversion.h"
#include "Math/Matrix.h"

#include "Common/Logger.h"
#include "Common/Constants.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace octronic::dream
{
    Transform::Transform
    ()
        : mTranslation(0.0f),
          mYaw(0.f),
          mPitch(0.f),
          mRoll(0.f),
          mScale(1.0f)
    {
        LOG_TRACE("Transform: Constructing");
    }

    Transform::Transform
    (const Transform& other)
    {
        LOG_TRACE("Transform: Constructing Copy");
        mTranslation = other.mTranslation;
        mYaw = other.mYaw;
        mPitch = other.mPitch;
        mRoll = other.mRoll;
        mScale = other.mScale;
    }

    Transform::Transform
    (const json& jsonTransform)
    {
        LOG_TRACE("Transform: Constructing from json");
        fromJson(jsonTransform);
    }

    Transform::~Transform() {}

    btTransform
    Transform::getBtTransform
    ()
    const
    {
        btTransform transform;
        transform.setFromOpenGLMatrix(glm::value_ptr(getMatrix()));
        return transform;
    }


    void Transform::operator=(const Transform& other)
    {
        mTranslation = other.mTranslation;
        mPitch = other.mPitch;
        mYaw = other.mYaw;
        mRoll = other.mRoll;
        mScale = other.mScale;
    }

    // JSON =====================================================================

    json
    Transform::toJson
    ()
    const
    {
        json j = json::object();
        j[Constants::TRANSLATION] = Vector3::toJson(mTranslation);
        j[Constants::YAW]         = mYaw;
        j[Constants::PITCH]       = mPitch;
        j[Constants::ROLL]        = mRoll;
        j[Constants::SCALE]       = Vector3::toJson(mScale);
        return j;
    }

    void Transform::fromJson(const json& js)
    {
        if (js.is_object())
        {
            if (js.find(Constants::TRANSLATION) != js.end())
            {
                mTranslation = Vector3::fromJson(js[Constants::TRANSLATION]);
            }

            if (js.find(Constants::YAW) != js.end())
            {
                mYaw = js[Constants::YAW];
            }

            if (js.find(Constants::PITCH) != js.end())
            {
                mPitch = js[Constants::PITCH];
            }

            if (js.find(Constants::ROLL) != js.end())
            {
                mRoll = js[Constants::ROLL];
            }

            if (js.find(Constants::SCALE) != js.end())
            {
                mScale = Vector3::fromJson(js[Constants::SCALE]);
            }
        }
    }

    float
    Transform::distanceFrom
    (const Transform& other)
    const
    {
        return glm::distance(mTranslation, other.mTranslation);
    }

    // Matrix ===================================================================

    mat4 Transform::getMatrix() const
    {
        static mat4 identity = mat4(1.f);
        mat4 translate = glm::translate(identity, mTranslation);
        mat4 scale     = glm::scale(identity, mScale);
        return translate * getOrientation() * scale;
    }

    void
    Transform::translate
    (const vec3& tx)
    {
        mTranslation += tx;
    }

    vec3
    Transform::getTranslation
    ()
    const
    {
        return mTranslation;
    }

    void Transform::setTranslation(const vec3& tx)
    {
        mTranslation = tx;
    }

    mat4 Transform::getOrientation() const
    {
        return glm::yawPitchRoll(mYaw,mPitch,mRoll);
    }

    void Transform::setYaw(float v)
    {
        mYaw = v;
    }

    void Transform::setPitch(float v)
    {
        mPitch = v;
    }

    void Transform::setRoll(float v)
    {
        mRoll = v;
    }

    float Transform::getYaw()
    const
    {
        return mYaw;
    }

    float Transform::getPitch()
    const
    {
        return mPitch;
    }

    float Transform::getRoll()
    const
    {
        return mRoll;
    }

    vec3 Transform::getScale() const
    {
        return mScale;
    }

    void Transform::setScale(const vec3&  sx)
    {
        mScale = sx;
    }
}


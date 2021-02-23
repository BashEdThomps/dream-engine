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

#pragma once

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <json.hpp>
#include <LinearMath/btTransform.h>

using glm::vec3;
using glm::mat4;
using nlohmann::json;

namespace octronic::dream
{
    /**
     * @brief Based around a glm::mat4 instance with convenience methods added
     * to make matrix manipulation easier.
     */
    class Transform
    {
    private:
        vec3 mTranslation;
        vec3 mScale;
        float mYaw;
        float mPitch;
        float mRoll;

    public:
        /**
         * @brief Default Constructor
         */
        Transform();


        /**
         * @brief Copy Constructor
         * @param other
         */
        Transform(const Transform& other);

        /**
         * @brief Construct by deserialising a matrix stored in JSON format.
         * @param j JSON matrix data
         */
        Transform(const json& j);

        /**
          * @brief Default Destructor
          */
        ~Transform();

        /**
         * @brief Get the matrix as a bullet transform object
         */
        btTransform getBtTransform() const;

        /**
         * @brief Get the matrix as a glm matrix object.
         */
        mat4 getMatrix() const;

        /**
         * @brief Translate the underlying matrix by the given amount.
         * @param translation Amount to translate the matrix by.
         */
        void translate(const vec3& translation);

        vec3 getTranslation() const;
        void setTranslation(const vec3&);

        mat4 getOrientation() const;

        void setYaw(float);
        void setPitch(float);
        void setRoll(float);
        float getYaw() const;
        float getPitch() const;
        float getRoll() const;

        vec3 getScale() const;
        void setScale(const vec3&);

        /**
         * @brief Get the JSON serialised representation of this object's
         * matrix.
         */
        json toJson() const;

        /**
         * @brief Populate the underlying matrix values from those given in the
         * JSON object.
         * @param js - json objct with matrix data
         */
        void fromJson(const json& js);

        /**
         * @param other Other transform
         * @return Distance between this and other Transform
         */
        float distanceFrom(const Transform& other) const;
    };
}

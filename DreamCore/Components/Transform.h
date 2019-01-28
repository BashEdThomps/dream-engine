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

#include "../deps/json/json.hpp"
#include <string>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>
#include "../Common/LockableObject.h"


using namespace std;
using namespace nlohmann;
using namespace glm;

namespace Dream
{
    /**
     * @brief The result of decomposing a glm::mat4 into it's constituent parts.
     */
    struct MatrixDecomposition
    {
        vec3 translation;
        quat rotation;
        vec3 scale;
        vec3 skew;
        vec4 perspective;
    };

    /**
     * @brief Based around a glm::mat4 instance with convenience methods added
     * to make matrix manipulation easier.
     */
    class Transform : public LockableObject
    {
    private:
        /**
         * @brief Internal matrix data.
         */
        mat4 mMatrix;

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
         * @brief Construct using the passed matrix data
         * @param fromMatrix data
         */
        Transform(const mat4& fromMatrix);

        /**
         * @brief Construct by deserialising a matrix stored in JSON format.
         * @param j JSON matrix data
         */
        explicit Transform(const json& j);

        /**
          * @brief Default Destructor
          */
        ~Transform() override;

        /**
         * @brief Get the matrix as a bullet transform object
         */
        btTransform getBtTransform() const;

        /**
         * @brief Get the matrix as a glm matrix object.
         */
        mat4 getMatrix() const;

        /**
         * @brief Set the internal matrix object from a glm matrix.
         * @param mat glm matrix object.
         */
        void setMatrix(const mat4& mat);

        /**
         * @brief Get the pointer to the float[16] matrix data array.
         */
        float* getMatrixFloatPointer();

        /**
         * @brief Decompose the matrix into it's constituent parts.
         * @see MatrixDecomposition
         */
        MatrixDecomposition decomposeMatrix(bool conjugate = false) const;

        /**
         * @brief Regenerate the matrix from it's constituent parts.
         * @param decomp Previously decomposed matrix data.
         * @param conjugate Conjugate the rotation matrix during decomposition.
         * @see MatrixDecomposition
         */
        void recomposeMatrix(MatrixDecomposition decomp, bool conjugate = false);

        /**
         * @brief Translate the underlying matrix by the given amount.
         * @param translation Amount to translate the matrix by.
         */
        void translate(const vec3& translation);

        /**
         * @brief Translate to this position first, then apply the underlyinng
         * matrix.
         * @param translation Amount to translate the matrix by.
         */
        void preTranslate(const vec3& translation);

        vec3 getTranslation() const;


        /**
         * @brief Get the JSON serialised representation of this object's
         * matrix.
         */
        json getJson() const;

        void fromJson(const json& js);

        /**
         * @param other Other transform
         * @return Distance between this and other Transform
         */
        float distanceFrom(const Transform& other) const;
    };
}

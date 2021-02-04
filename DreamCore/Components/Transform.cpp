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

#include "Common/Math.h"
#include "Common/Logger.h"
#include "Common/Constants.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>



namespace octronic::dream
{

    Transform::Transform
    ()
        : LockableObject("Transform"),
          mMatrix(1.0f)
    {
        LOG_TRACE("Transform: Constructing");
    }

    Transform::Transform
    (const Transform& other)
        :LockableObject ("Transform")
    {
        LOG_TRACE("Transform: Constructing Copy");
        mMatrix = other.mMatrix;
    }

    Transform::Transform
    (const mat4& mtx)
        : LockableObject("Transform")
    {
        LOG_TRACE("Transform: Constructing from matrix");
        setMatrix(mtx);
    }

    Transform::Transform
    (const json& jsonTransform)
        : LockableObject("Transform")
    {
        LOG_TRACE("Transform: Constructing from json");
        fromJson(jsonTransform);
    }

    Transform::~Transform() {}

    btTransform
    Transform::getBtTransform
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            btTransform transform;
            transform.setFromOpenGLMatrix(value_ptr(mMatrix));
            return transform;

        } dreamElseLockFailed
    }

    // JSON =====================================================================

    json
    Transform::getJson
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            json j = json::object();
            // Translation
            j[Constants::TRANSFORM_MATRIX] = json::array();
            for (int i=0; i<16; i++)
            {
                j[Constants::TRANSFORM_MATRIX].push_back(glm::value_ptr(mMatrix)[i]);
            }

            return j;

        } dreamElseLockFailed
    }

    void Transform::fromJson(const json& jsonTransform)
    {
        if(dreamTryLock())
        {
            dreamLock();
            if (jsonTransform.find(Constants::TRANSFORM_MATRIX) != jsonTransform.end())
            {
                float mtxFloat[16] = {0.0f};
                for (int i=0; i<16; i++)
                {
                    mtxFloat[i] = (float)jsonTransform[Constants::TRANSFORM_MATRIX][i];
                    mMatrix = glm::make_mat4(mtxFloat);
                }
            }
            else
            {
                mMatrix = mat4(1.0f);
            }

        } dreamElseLockFailed
    }

    float
    Transform::distanceFrom
    (const Transform& other)
    {
        if(dreamTryLock())
        {
            dreamLock();
            return glm::distance(mMatrix[3], other.mMatrix[3]);

        } dreamElseLockFailed
    }

    // Matrix ===================================================================

    void
    Transform::setMatrix
    (const mat4& mtx)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mMatrix = mtx;

        } dreamElseLockFailed
    }

    float*
    Transform::getMatrixFloatPointer
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return (float*)(&mMatrix[0]);

        } dreamElseLockFailed
    }

    mat4 Transform::getMatrix()
    {
        if(dreamTryLock())
        {
            dreamLock();
            return mMatrix;

        } dreamElseLockFailed
    }

    MatrixDecomposition
    Transform::decomposeMatrix
    (bool conjugate)
    {
        if(dreamTryLock())
        {
            dreamLock();
            MatrixDecomposition decomp;
            glm::decompose(mMatrix, decomp.scale, decomp.rotation, decomp.translation, decomp.skew, decomp.perspective);
            if (conjugate)
            {
                decomp.rotation = glm::conjugate(decomp.rotation);
            }
            return decomp;

        } dreamElseLockFailed
    }

    void
    Transform::recomposeMatrix
    (MatrixDecomposition decomp,bool conjugate)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mMatrix = mat4(1.0f);
            if (conjugate)
            {
                mMatrix = mat4_cast(glm::conjugate(decomp.rotation))*mMatrix;
            }
            else
            {
                mMatrix = mat4_cast(decomp.rotation)*mMatrix;
            }
            mMatrix = glm::translate(mMatrix,decomp.translation);
            mMatrix = glm::scale(mMatrix,decomp.scale);

        } dreamElseLockFailed
    }

    void
    Transform::translate
    (const Vector3& tx)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mMatrix = glm::translate(mMatrix,tx.toGLM());

        } dreamElseLockFailed
    }

    void
    Transform::translate3f
    (float x, float y, float z)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mMatrix = glm::translate(mMatrix,vec3(x,y,z));

        } dreamElseLockFailed
    }


    void
    Transform::preTranslate
    (const Vector3& translation)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mat4 mat = glm::translate(mat4(1.0f), translation.toGLM());
            mMatrix = mat*mMatrix;

        } dreamElseLockFailed
    }

    Vector3&
    Transform::getTranslation
    ()
    {
        if(dreamTryLock())
        {
            dreamLock();
            static Vector3 tx;
            tx = Vector3(
                        mMatrix[3].x,
                    mMatrix[3].y,
                    mMatrix[3].z
                    );
            return tx;

        } dreamElseLockFailed
    }
}

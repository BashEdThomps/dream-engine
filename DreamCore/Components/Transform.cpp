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

#define GLM_ENABLE_EXPERIMENTAL

#include "Transform.h"

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

using namespace glm;

namespace Dream
{

    Transform::Transform
    ()
        : DreamObject("Transform"),
          mMatrix(1.0f)
    {}

    Transform::Transform
    (mat4 mtx)
        : DreamObject("Transform")
    {
        setMatrix(mtx);
    }

    Transform::Transform
    (json jsonTransform)
        : DreamObject("Transform")
    {
        if (!jsonTransform[Constants::TRANSFORM_MATRIX].is_null())
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
    }

    Transform::~Transform() {}

    btTransform
    Transform::getBtTransform
    ()
    const
    {
        btTransform transform;
        float tmp[16];
        const float* matPtr = value_ptr(mMatrix);
        memcpy(tmp,matPtr,sizeof(float)*16);
        transform.setFromOpenGLMatrix(tmp);
        return transform;
    }

    // JSON =====================================================================

    json
    Transform::getJson
    ()
    const
    {
        json j = json::object();
        // Translation
        j[Constants::TRANSFORM_MATRIX] = json::array();
        for (int i=0; i<16; i++)
        {
            j[Constants::TRANSFORM_MATRIX].push_back(glm::value_ptr(mMatrix)[i]);
        }
        return j;
    }

    // Matrix ===================================================================

    void
    Transform::setMatrix
    (mat4 mtx)
    {
        mMatrix = mtx;
    }

    float*
    Transform::getMatrixFloatPointer
    ()
    {
        return (float*)(&mMatrix[0]);
    }

    mat4&
    Transform::getMatrix
    ()
    {
        return mMatrix;
    }

    MatrixDecomposition
    Transform::decomposeMatrix
    ()
    {
        MatrixDecomposition decomp;
        glm::decompose(mMatrix, decomp.scale, decomp.rotation, decomp.translation, decomp.skew, decomp.perspective);
        //decomp.rotation = glm::conjugate(decomp.rotation);
        return decomp;
    }

    void
    Transform::recomposeMatrix
    (MatrixDecomposition decomp)
    {
        mMatrix = mat4(1.0f);
        mMatrix = mat4_cast(decomp.rotation)*mMatrix;
        mMatrix = glm::translate(mMatrix,decomp.translation);
        mMatrix = glm::scale(mMatrix,decomp.scale);
    }

    void
    Transform::translate
    (vec3 tx)
    {
       mMatrix = glm::translate(mMatrix,tx);
    }

} // End of Dream

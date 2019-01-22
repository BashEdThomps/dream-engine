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
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->trace("Constructing");
        #endif
    }

    Transform::Transform(const Transform& other)
        :DreamObject ("Transform")
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->trace("Constructing Copy");
        #endif
        mMatrix = other.mMatrix;
    }

    Transform::Transform
    (mat4 mtx)
        : DreamObject("Transform")
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->trace("Constructing from matrix");
        #endif
        setMatrix(mtx);
    }

    Transform::Transform
    (json jsonTransform)
        : DreamObject("Transform")
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->trace("Constructing from json");
        #endif
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

    float
    Transform::distanceFrom
    (const Transform& other)
    {
       return glm::distance(mMatrix[3], other.mMatrix[3]);
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
    (bool conjugate)
    const
    {
        MatrixDecomposition decomp;
        glm::decompose(mMatrix, decomp.scale, decomp.rotation, decomp.translation, decomp.skew, decomp.perspective);
        if (conjugate)
        {
            decomp.rotation = glm::conjugate(decomp.rotation);
        }
        return decomp;
    }

    void
    Transform::recomposeMatrix
    (MatrixDecomposition decomp,bool conjugate)
    {
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
    }

    void
    Transform::translate
    (vec3 tx)
    {
        mMatrix = glm::translate(mMatrix,tx);
    }

    void
    Transform::preTranslate
    (vec3 translation)
    {
        mat4 mat = glm::translate(mat4(1.0f), translation);
        mMatrix = mat*mMatrix;
    }

} // End of Dream

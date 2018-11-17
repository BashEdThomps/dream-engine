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
    struct MatrixDecomposition
    {
        vec3 translation;
        quat rotation;
        vec3 scale;
        vec3 skew;
        vec4 perspective;
    };

    class Transform : public DreamObject
    {
    private:
        mat4 mMatrix;

    public:
        Transform();
        Transform(mat4 fromMatrix);
        explicit Transform(json j);
        ~Transform() override;

        btTransform getBtTransform() const;

        mat4& getMatrix();
        void setMatrix(mat4 mat);
        float* getMatrixFloatPointer();
        MatrixDecomposition decomposeMatrix();
        void recomposeMatrix(MatrixDecomposition decomp);

        json getJson();
    };

} // End of Dream

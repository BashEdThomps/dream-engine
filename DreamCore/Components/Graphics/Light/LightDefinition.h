/*
 * LightDefinition.h
 *
 * Created: 11 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
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

#include "../../AssetDefinition.h"
#include <glm/vec3.hpp>

using namespace glm;

namespace Dream
{
    enum LightType
    {
        LT_NONE,
        LT_POINT,
        LT_DIRECTIONAL,
        LT_SPOTLIGHT
    };

    class LightDefinition : public AssetDefinition
    {
    public:
        LightDefinition(ProjectDefinition* pd, const json& js);
        ~LightDefinition() override;

        void setDiffuse(vec3);
        vec3 getDiffuse();

        void setAmbient(vec3);
        vec3 getAmbient();

        void setSpecular(vec3);
        vec3 getSpecular();

        void setType(LightType);
        LightType getType();

        void setCutOff(float);
        float getCutOff();

        void setOuterCutOff(float);
        float getOuterCutOff();

        void setConstant(float);
        float getConstant();

        void setLinear(float);
        float getLinear();

        void setQuadratic(float);
        float getQuadratic();

        float getSpecularRed() ;
        void setSpecularRed(float specularRed);
        float getSpecularGreen() ;
        void setSpecularGreen(float specularGreen);
        float getSpecularBlue() ;
        void setSpecularBlue(float specularBlue);

        float getAmbientRed() ;
        void setAmbientRed(float ambientRed);
        float getAmbientGreen() ;
        void setAmbientGreen(float ambientGreen);
        float getAmbientBlue() ;
        void setAmbientBlue(float ambientBlue);

        float getDiffuseRed() ;
        void setDiffuseRed(float diffuseRed);
        float getDiffuseGreen() ;
        void setDiffuseGreen(float diffuseGreen);
        float getDiffuseBlue() ;
        void setDiffuseBlue(float diffuseBlue);
    };
}

/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once

#include <glm/glm.hpp>
#include "LightDefinition.h"
#include "../../DiscreteAssetRuntime.h"

using namespace glm;

namespace Dream
{
    struct DirLight
    {
        vec3 direction;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    };

    struct PointLight
    {
        vec3 position;
        float constant;
        float linear;
        float quadratic;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    };

    struct SpotLight
    {
        vec3 position;
        vec3 direction;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float cutOff;
        float outerCutOff;
        float constant;
        float linear;
        float quadratic;
    };

    class LightDefinition;

    class LightRuntime
        : public DiscreteAssetRuntime
    {
        vec3 mAmbient;
        vec3 mDiffuse;
        vec3 mSpecular;
        float mConstant;
        float mLinear;
        float mQuadratic;
        float mCutOff;
        float mOuterCutOff;
        LightType mType;

    public:
        LightRuntime(
            LightDefinition*,
            SceneObjectRuntime*
        );
        ~LightRuntime() override;

        bool useDefinition() override;

        vec3 getAmbient() const;
        void setAmbient(const vec3& ambient);

        vec3 getDiffuse() const;
        void setDiffuse(const vec3& diffuse);

        vec3 getSpecular() const;
        void setSpecular(const vec3& specular);

        float getConstant() const;
        void setConstant(float constant);

        float getLinear() const;
        void setLinear(float linear);

        float getQuadratic() const;
        void setQuadratic(float quadratic);

        float getCutOff() const;
        void setCutOff(float cutOff);

        float getOuterCutOff() const;
        void setOuterCutOff(float outerCutOff);

        LightType getType() const;
        void setType(const LightType& type);
        void loadType();

        PointLight getPointLightData();
        SpotLight getSpotLightData();
        DirLight getDirectionalLightData();

    protected:

    };
}

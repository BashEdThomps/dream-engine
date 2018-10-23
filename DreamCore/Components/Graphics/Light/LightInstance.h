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
#include "../../IAssetInstance.h"
#include "LightDefinition.h"

using glm::vec3;

namespace Dream
{
    typedef struct
    {
        float constant;
        float linear;
        float quadratic;
    } KMap;

    typedef struct {
        vec3 direction;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    } DirLight;

    typedef struct {
        vec3 position;

        float constant;
        float linear;
        float quadratic;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    } PointLight;

    typedef struct {
        vec3 position;
        vec3 direction;
        float cutOff;
        float outerCutOff;

        float constant;
        float linear;
        float quadratic;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    } SpotLight;

    class LightDefinition;
    class LightInstance : public IAssetInstance
    {
        vec3 mAmbient;
        vec3 mDiffuse;
        vec3 mSpecular;
        vec3 mDirection;
        float mConstant;
        float mLinear;
        float mQuadratic;
        float mCutOff;
        float mOuterCutOff;
        LightType mType;

    public:
        LightInstance(
            LightDefinition*,
            SceneObjectRuntime*
        );
        ~LightInstance() override;

        bool load(string) override;
        void loadExtraAttributes(json) override;

        static KMap getKMap(int distance);
        static map<int, KMap> K_VALUE_MAP;

        vec3 getAmbient() const;
        void setAmbient(const vec3& ambient);

        vec3 getDiffuse() const;
        void setDiffuse(const vec3& diffuse);

        vec3 getSpecular() const;
        void setSpecular(const vec3& specular);

        vec3 getDirection() const;
        void setDirection(const vec3& direction);

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

    }; // End of LightInstance

} // End of Dream

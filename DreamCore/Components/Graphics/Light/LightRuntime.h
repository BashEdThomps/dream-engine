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
#include "LightTasks.h"
#include "Components/DiscreteAssetRuntime.h"

namespace octronic::dream
{
    struct DirLight
    {
        Vector3 direction;
        Vector3 ambient;
        Vector3 diffuse;
        Vector3 specular;
    };

    struct PointLight
    {
        Vector3 position;
        float constant;
        float linear;
        float quadratic;
        Vector3 ambient;
        Vector3 diffuse;
        Vector3 specular;
    };

    struct SpotLight
    {
        Vector3 position;
        Vector3 direction;
        Vector3 ambient;
        Vector3 diffuse;
        Vector3 specular;
        float cutOff;
        float outerCutOff;
        float constant;
        float linear;
        float quadratic;
    };

    class LightDefinition;

    class LightRuntime : public DiscreteAssetRuntime
    {


    public:
        LightRuntime(ProjectRuntime*, LightDefinition*,EntityRuntime*);
        ~LightRuntime() override;

        bool loadFromDefinition() override;

        Vector3 getAmbient() const;
        void setAmbient(const Vector3& ambient);

        Vector3 getDiffuse() const;
        void setDiffuse(const Vector3& diffuse);

        Vector3 getSpecular() const;
        void setSpecular(const Vector3& specular);

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

        PointLight getPointLightData() const;
        SpotLight getSpotLightData() const;
        DirLight getDirectionalLightData() const;

        void pushNextTask() override;

    private:
       	shared_ptr<LightAddToQueueTask> mLightAddToQueueTask;
        Vector3 mAmbient;
        Vector3 mDiffuse;
        Vector3 mSpecular;
        float mConstant;
        float mLinear;
        float mQuadratic;
        float mCutOff;
        float mOuterCutOff;
        LightType mType;
    };
}

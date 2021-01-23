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

#include "Components/SharedAssetRuntime.h"

#include <vector>
#include <glm/vec3.hpp>

using std::vector;
using glm::vec3;

namespace octronic::dream
{
    struct RGB;
    class ModelMesh;
    class ShaderRuntime;
    class MaterialDefinition;
    class TextureRuntime;
    class Camera;

    class MaterialRuntime : public SharedAssetRuntime
    {
    public:
        MaterialRuntime(MaterialDefinition* def, ProjectRuntime* rt);
        ~MaterialRuntime() override;

        void addMesh(ModelMesh* mesh);
        void clearMeshes();
        size_t countMeshes();

        bool operator==(MaterialRuntime& other);

        void debug();
        void logMeshes();

        void drawGeometryPass(Camera*);
        void drawShadowPass(ShaderRuntime* shader);

        bool useDefinition() override;

        TextureRuntime* getDiffuseTexture() const;
        void setDiffuseTexture(TextureRuntime* diffuseTexture);

        TextureRuntime* getSpecularTexture() const;
        void setSpecularTexture(TextureRuntime* specularTexture);

        TextureRuntime* getNormalTexture() const;
        void setNormalTexture(TextureRuntime* normalTexture);

        TextureRuntime* getDisplacementTexture() const;
        void setDisplacementTexture(TextureRuntime* displacementTexture);

        ShaderRuntime* getShader() const;
        void setShader(ShaderRuntime* shader);

        vec3 getColorDiffuse() const;
        void setColorDiffuse(vec3 colorDiffuse);

        vec3 getColorAmbient() const;
        void setColorAmbient(vec3 colorAmbient);

        vec3 getColorSpecular() const;
        void setColorSpecular(vec3 colorSpecular);

        vec3 getColorEmissive() const;
        void setColorEmissive(vec3 colorEmissive);

        vec3 getColorReflective() const;
        void setColorReflective(vec3 colorReflective);

        float getShininessStrength() const;
        void setShininessStrength(float shininessStrength);

        bool getIgnore() const;
        void setIgnore(bool ignore);

    protected:
        float mOpacity = 0.0f;
        float mBumpScaling = 0.0f;
        float mHardness = 0.0f;
        float mReflectivity = 0.0f;
        float mShininessStrength = 0.0f;
        float mRefracti = 0.0f;
        bool mIgnore = false;

        vec3 mColorDiffuse;
        vec3 mColorAmbient;
        vec3 mColorSpecular;
        vec3 mColorEmissive;
        vec3 mColorReflective;

        TextureRuntime* mDiffuseTexture;
        TextureRuntime* mSpecularTexture;
        TextureRuntime* mNormalTexture;
        TextureRuntime* mDisplacementTexture;
        ShaderRuntime*  mShader;

        vector<ModelMesh*> mUsedBy;

        vec3 rgbToVec3(RGB color);
    };
}

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
    class ModelMesh;
    class ShaderRuntime;
    class MaterialDefinition;
    class TextureRuntime;
    class CameraRuntime;

    class MaterialRuntime : public SharedAssetRuntime
    {
    public:
        MaterialRuntime(ProjectRuntime* rt, MaterialDefinition* def);
        ~MaterialRuntime() override;

        void addMesh(ModelMesh* mesh);
        void removeMesh(ModelMesh* mesh);
        void clearMeshes();
        size_t countMeshes();

        bool operator==(MaterialRuntime& other);

        void debug();
        void logMeshes();

        void drawShadowPass(ShaderRuntime* shader);

        bool loadFromDefinition() override;

        TextureRuntime* getAlbedoTextureHandle() const;
        void setAlbedoTextureHandle(TextureRuntime* texture);

        TextureRuntime* getNormalTextureHandle() const;
        void setNormalTextureHandle(TextureRuntime* texture);

        TextureRuntime* getMetallicTextureHandle() const;
        void setMetallicTextureHandle(TextureRuntime* texture);

        TextureRuntime* getRoughnessTextureHandle() const;
        void setRoughnessTextureHandle(TextureRuntime* texture);

        TextureRuntime* getAoTextureHandle() const;
        void setAoTextureHandle(TextureRuntime* texture);

        ShaderRuntime* getShaderHandle() const;
        void setShaderHandle(ShaderRuntime* shader);

        void pushTasks() override;

        // Used because InstanceVector is of type Entity*
        vector<ModelMesh*>& getUsedByVector();

    protected:
        TextureRuntime* mAlbedoTextureHandle;
        TextureRuntime* mNormalTextureHandle;
        TextureRuntime* mMetallicTextureHandle;
        TextureRuntime* mRoughnessTextureHandle;
        TextureRuntime* mAoTextureHandle;
        ShaderRuntime*  mShaderHandle;

        // Used as InstanceVector is Entity*
        vector<ModelMesh*> mUsedBy;
    };
}

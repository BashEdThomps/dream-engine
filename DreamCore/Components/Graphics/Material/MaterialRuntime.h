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
        MaterialRuntime
        (const weak_ptr<ProjectRuntime>& rt,
         const weak_ptr<MaterialDefinition>& def);
        ~MaterialRuntime() override;

        void addMesh(const weak_ptr<ModelMesh>& mesh);
        void removeMesh(const weak_ptr<ModelMesh>& mesh);
        void clearMeshes();
        size_t countMeshes();

        bool operator==(MaterialRuntime& other);

        void debug();
        void logMeshes();

        void drawShadowPass(const weak_ptr<ShaderRuntime>& shader);

        bool loadFromDefinition() override;

        weak_ptr<TextureRuntime> getAlbedoTexture() const;
        void setAlbedoTexture(const weak_ptr<TextureRuntime>& texture);

        weak_ptr<TextureRuntime> getNormalTexture() const;
        void setNormalTexture(const weak_ptr<TextureRuntime>& texture);

        weak_ptr<TextureRuntime> getMetallicTexture() const;
        void setMetallicTexture(const weak_ptr<TextureRuntime>& texture);

        weak_ptr<TextureRuntime> getRoughnessTexture() const;
        void setRoughnessTexture(const weak_ptr<TextureRuntime>& texture);

        weak_ptr<TextureRuntime> getAoTexture() const;
        void setAoTexture(const weak_ptr<TextureRuntime>& texture);

        weak_ptr<ShaderRuntime> getShader() const;
        void setShader(const weak_ptr<ShaderRuntime>& shader);

        void pushTasks() override;

        // Used because InstanceVector is of type Entity*
        vector<weak_ptr<ModelMesh>> getUsedByVector() const;

    protected:
        weak_ptr<TextureRuntime> mAlbedoTexture;
        weak_ptr<TextureRuntime> mNormalTexture;
        weak_ptr<TextureRuntime> mMetallicTexture;
        weak_ptr<TextureRuntime> mRoughnessTexture;
        weak_ptr<TextureRuntime> mAoTexture;
        weak_ptr<ShaderRuntime>  mShader;

        // Used as InstanceVector is Entity*
        vector<weak_ptr<ModelMesh>> mUsedBy;
    };
}

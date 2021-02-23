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
#include "MaterialRuntime.h"

#include "MaterialDefinition.h"
#include "Common/Logger.h"
#include "Components/Graphics/Shader/ShaderRuntime.h"
#include "Components/Graphics/Texture/TextureRuntime.h"
#include "Components/Graphics/Model/ModelMesh.h"
#include "Components/Graphics/Camera.h"
#include "Project/ProjectRuntime.h"
#include "Components/Cache.h"

namespace octronic::dream
{
    MaterialRuntime::MaterialRuntime(ProjectRuntime* rt, MaterialDefinition* def)
        : SharedAssetRuntime(rt, def),
          mAlbedoTextureHandle(nullptr),
          mNormalTextureHandle(nullptr),
          mMetallicTextureHandle(nullptr),
          mRoughnessTextureHandle(nullptr),
          mAoTextureHandle(nullptr),
          mShaderHandle(nullptr)
    {
        LOG_TRACE("MaterialRuntime: Constructing");
    }

    MaterialRuntime::~MaterialRuntime()
    {
        LOG_TRACE("MaterialRuntime: Destructing");
    }

    void
    MaterialRuntime::addMesh
    (ModelMesh* mesh)
    {
        mUsedBy.push_back(mesh);
    }

    void
    MaterialRuntime::removeMesh
    (ModelMesh *mesh)
    {
       auto itr = std::find(mUsedBy.begin(), mUsedBy.end(), mesh);
       if (itr != mUsedBy.end())
       {
           mUsedBy.erase(itr);
       }
    }

    void
    MaterialRuntime::clearMeshes
    ()
    {
        mUsedBy.clear();
    }

    size_t
    MaterialRuntime::countMeshes
    ()
    {
        return mUsedBy.size();
    }

    bool
    MaterialRuntime::operator==
    (MaterialRuntime& other)
    {
        return getName() == other.getName();
    }

    void
    MaterialRuntime::debug
    ()
    {
        GLuint albedo, normal, metallic, roughness, ao;
        albedo    = (mAlbedoTextureHandle    == nullptr ? 0 : mAlbedoTextureHandle->getTextureID());
        normal    = (mNormalTextureHandle    == nullptr ? 0 : mNormalTextureHandle->getTextureID());
        metallic  = (mMetallicTextureHandle  == nullptr ? 0 : mMetallicTextureHandle->getTextureID());
        roughness = (mRoughnessTextureHandle == nullptr ? 0 : mRoughnessTextureHandle->getTextureID());
        ao        = (mAoTextureHandle        == nullptr ? 0 : mAoTextureHandle->getTextureID());

        LOG_TRACE("Maerial Parameters\n"
            "Name....................{}\n"
            "AlbedoTexture...........{}\n"
            "NormalTexture...........{}\n"
            "MetallicTexture.........{}\n"
            "RoughnessTexture........{}\n"
            "AoTexture...............{}\n"
            "Meshes..................{}",
            getName(), albedo, normal,
            metallic, roughness, ao,
            mUsedBy.size());
    }

    void
    MaterialRuntime::logMeshes
    ()
    {
        LOG_DEBUG("\tMeshes for material {} : {}",getName(),mUsedBy.size());
        for (auto mesh : mUsedBy)
        {
            LOG_DEBUG("\t\t{}", mesh->getName());
            mesh->logRuntimes();
        }
    }

    vector<ModelMesh*>& MaterialRuntime::getUsedByVector()
    {
        return mUsedBy;
    }

    void
    MaterialRuntime::drawShadowPass
    (ShaderRuntime* shader)
    {
        for (auto mesh : mUsedBy)
        {
            mesh->drawShadowPassRuntimes(shader);
        }
    }

    bool
    MaterialRuntime::loadFromDefinition
    ()
    {
        auto matDef = static_cast<MaterialDefinition*>(mDefinitionHandle);

        // Shaders & Textures
        auto shaderCache = mProjectRuntimeHandle->getShaderCache();
        auto textureCache = mProjectRuntimeHandle->getTextureCache();
        mShaderHandle = shaderCache->getRuntimeHandle(matDef->getShader());

        if (mShaderHandle == nullptr)
        {
            LOG_ERROR("MaterialCache: Cannot create material {} with null shader", matDef->getNameAndUuidString());
            return false;
        }

        mAlbedoTextureHandle = textureCache->getRuntimeHandle(matDef->getAlbedoTexture());
        mNormalTextureHandle = textureCache->getRuntimeHandle(matDef->getNormalTexture());
        mMetallicTextureHandle = textureCache->getRuntimeHandle(matDef->getMetallicTexture());
        mRoughnessTextureHandle = textureCache->getRuntimeHandle(matDef->getRoughnessTexture());
        mAoTextureHandle = textureCache->getRuntimeHandle(matDef->getAoTexture());
        mShaderHandle->addMaterial(this);
        return true;
    }

    ShaderRuntime*
    MaterialRuntime::getShaderHandle
    () const
    {
        return mShaderHandle;
    }

    void
    MaterialRuntime::setShaderHandle
    (ShaderRuntime* shader)
    {
        mShaderHandle = shader;
    }

    TextureRuntime*
    MaterialRuntime::getAlbedoTextureHandle
    () const
    {
        return mAlbedoTextureHandle;
    }

    void
    MaterialRuntime::setAlbedoTextureHandle
    (TextureRuntime* t)
    {
        mAlbedoTextureHandle = t;
    }

    TextureRuntime*
    MaterialRuntime::getNormalTextureHandle
    () const
    {
        return mNormalTextureHandle;
    }

    void
    MaterialRuntime::setNormalTextureHandle
    (TextureRuntime* normalTexture)
    {
        mNormalTextureHandle = normalTexture;
    }

    TextureRuntime*
    MaterialRuntime::getMetallicTextureHandle
    () const
    {
        return mMetallicTextureHandle;
    }

    void
    MaterialRuntime::setMetallicTextureHandle
    (TextureRuntime* t)
    {
        mMetallicTextureHandle = t;
    }

    TextureRuntime*
    MaterialRuntime::getRoughnessTextureHandle
    () const
    {
        return mRoughnessTextureHandle;
    }

    void
    MaterialRuntime::setRoughnessTextureHandle
    (TextureRuntime* t)
    {
        mRoughnessTextureHandle = t;
    }

    TextureRuntime*
    MaterialRuntime::getAoTextureHandle
    () const
    {
        return mAoTextureHandle;
    }

    void
    MaterialRuntime::setAoTextureHandle
    (TextureRuntime* t)
    {
        mAoTextureHandle = t;
    }

    void MaterialRuntime::pushTasks()
    {
        auto taskQueue = mProjectRuntimeHandle->getTaskQueue();

        if (!mLoaded && !mLoadError && mLoadFromDefinitionTask->hasState(TASK_STATE_QUEUED))
        {
            taskQueue->pushTask(mLoadFromDefinitionTask);
        }
    }
}

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
#include "Components/Graphics/CameraRuntime.h"
#include "Project/ProjectRuntime.h"
#include "Components/Cache.h"

using std::static_pointer_cast;

namespace octronic::dream
{
  MaterialRuntime::MaterialRuntime(
      const weak_ptr<ProjectRuntime>& rt,
      const weak_ptr<MaterialDefinition>& def)
    : SharedAssetRuntime(rt, def)
  {
    LOG_TRACE("MaterialRuntime: Constructing");
  }

  MaterialRuntime::~MaterialRuntime()
  {
    LOG_TRACE("MaterialRuntime: Destructing");
  }

  void
  MaterialRuntime::addMesh
  (const weak_ptr<ModelMesh>& mesh)
  {
    mUsedBy.push_back(mesh);
  }

  void
  MaterialRuntime::removeMesh
  (const weak_ptr<ModelMesh>& mesh)
  {
    auto itr = std::find_if(mUsedBy.begin(), mUsedBy.end(),
    	[&](const weak_ptr<ModelMesh>& next){
      	return next.lock() == mesh.lock();
    	});
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
    albedo    = (mAlbedoTexture.expired()    ? 0 : mAlbedoTexture.lock()->getTextureID());
    normal    = (mNormalTexture.expired()    ? 0 : mNormalTexture.lock()->getTextureID());
    metallic  = (mMetallicTexture.expired()  ? 0 : mMetallicTexture.lock()->getTextureID());
    roughness = (mRoughnessTexture.expired() ? 0 : mRoughnessTexture.lock()->getTextureID());
    ao        = (mAoTexture.expired()        ? 0 : mAoTexture.lock()->getTextureID());

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
      if (auto meshLock = mesh.lock())
      {
        LOG_DEBUG("\t\t{}", meshLock->getName());
        meshLock->logRuntimes();
      }
    }
  }

  vector<weak_ptr<ModelMesh>>
  MaterialRuntime::getUsedByVector()
  const
  {
    return mUsedBy;
  }

  void
  MaterialRuntime::drawShadowPass
  (const weak_ptr<ShaderRuntime>& shader)
  {
    for (auto mesh : mUsedBy)
    {
      if (auto meshLock = mesh.lock())
      {
        meshLock->drawShadowPassRuntimes(shader);
      }
    }
  }

  bool
  MaterialRuntime::loadFromDefinition
  ()
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto defLock = mDefinition.lock())
      {
        auto matDef = static_pointer_cast<MaterialDefinition>(defLock);

        // Shaders & Textures
        if (auto shaderCache = prLock->getShaderCache().lock())
        {

          if (auto textureCache = prLock->getTextureCache().lock())
          {
            mShader = shaderCache->getRuntime(matDef->getShader());

            if (mShader.expired())
            {
              LOG_ERROR("MaterialCache: Cannot create material {} with null shader", matDef->getNameAndUuidString());
              return false;
            }

            if (auto shaderLock = mShader.lock())
            {
              mAlbedoTexture = textureCache->getRuntime(matDef->getAlbedoTexture());
              mNormalTexture = textureCache->getRuntime(matDef->getNormalTexture());
              mMetallicTexture = textureCache->getRuntime(matDef->getMetallicTexture());
              mRoughnessTexture = textureCache->getRuntime(matDef->getRoughnessTexture());
              mAoTexture = textureCache->getRuntime(matDef->getAoTexture());
              shaderLock->addMaterial(static_pointer_cast<MaterialRuntime>(shared_from_this()));
              return true;
            }
          }
        }
      }
    }
    return false;
  }

  weak_ptr<ShaderRuntime>
  MaterialRuntime::getShader
  () const
  {
    return mShader;
  }

  void
  MaterialRuntime::setShader
  (const weak_ptr<ShaderRuntime>& shader)
  {
    mShader = shader;
  }

  weak_ptr<TextureRuntime>
  MaterialRuntime::getAlbedoTexture
  () const
  {
    return mAlbedoTexture;
  }

  void
  MaterialRuntime::setAlbedoTexture
  (const weak_ptr<TextureRuntime>& t)
  {
    mAlbedoTexture = t;
  }

  weak_ptr<TextureRuntime>
  MaterialRuntime::getNormalTexture
  () const
  {
    return mNormalTexture;
  }

  void
  MaterialRuntime::setNormalTexture
  (const weak_ptr<TextureRuntime>& normalTexture)
  {
    mNormalTexture = normalTexture;
  }

  weak_ptr<TextureRuntime>
  MaterialRuntime::getMetallicTexture
  () const
  {
    return mMetallicTexture;
  }

  void
  MaterialRuntime::setMetallicTexture
  (const weak_ptr<TextureRuntime>& t)
  {
    mMetallicTexture = t;
  }

  weak_ptr<TextureRuntime>
  MaterialRuntime::getRoughnessTexture
  () const
  {
    return mRoughnessTexture;
  }

  void
  MaterialRuntime::setRoughnessTexture
  (const weak_ptr<TextureRuntime>& t)
  {
    mRoughnessTexture = t;
  }

  weak_ptr<TextureRuntime>
  MaterialRuntime::getAoTexture
  () const
  {
    return mAoTexture;
  }

  void
  MaterialRuntime::setAoTexture
  (const weak_ptr<TextureRuntime>& t)
  {
    mAoTexture = t;
  }

  void
  MaterialRuntime::pushTasks
  ()
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto taskQueue = prLock->getTaskQueue().lock())
      {
        if (!mLoaded && !mLoadError && mLoadFromDefinitionTask->hasState(TASK_STATE_QUEUED))
        {
          taskQueue->pushTask(mLoadFromDefinitionTask);
        }
      }
    }
  }
}

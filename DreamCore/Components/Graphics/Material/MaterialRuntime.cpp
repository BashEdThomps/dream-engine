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
          mColorDiffuse(vec3(0.0f)),
          mColorAmbient(vec3(0.0f)),
          mColorSpecular(vec3(0.0f)),
          mColorEmissive(vec3(0.0f)),
          mColorReflective(vec3(0.0f)),
          mDiffuseTextureHandle(nullptr),
          mSpecularTextureHandle(nullptr),
          mNormalTextureHandle(nullptr),
          mDisplacementTextureHandle(nullptr),
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
        GLuint diff, spec, norm, disp;
        diff = (mDiffuseTextureHandle  == nullptr ? 0 : mDiffuseTextureHandle->getGLID());
        spec = (mSpecularTextureHandle == nullptr ? 0 : mSpecularTextureHandle->getGLID());
        norm = (mNormalTextureHandle   == nullptr ? 0 : mNormalTextureHandle->getGLID());
        disp = (mDisplacementTextureHandle == nullptr ? 0 : mDisplacementTextureHandle->getGLID());

        LOG_TRACE(
                    "Maerial Parameters\n"
            "Name....................{}\n"
            "Opacity.................{}\n"
            "BumpScaling.............{}\n"
            "Hardness................{}\n"
            "Reflectivity............{}\n"
            "ShininessStrength.......{}\n"
            "RefractI................{}\n"
            "Ignore..................{}\n"

            "DiffuseColour...........({},{},{})\n"
            "AmbientColour...........({},{},{})\n"
            "SpecularColour..........({},{},{})\n"
            "EmissiveColour..........({},{},{})\n"
            "ReflectiveColour........({},{},{})\n"

            "DiffuseTexture..........{}\n"
            "SpecularTexture.........{}\n"
            "NormalTexture...........{}\n"
            "DisplacementTexture.....{}\n"
            "Meshes..................{}",
                    getName(),
                    mOpacity,
                    mBumpScaling,
                    mHardness,
                    mReflectivity,
                    mShininessStrength,
                    mRefracti,
                    mIgnore,

                    mColorDiffuse.r, mColorDiffuse.g, mColorDiffuse.b,
                    mColorAmbient.r, mColorAmbient.g, mColorAmbient.b,
                    mColorSpecular.r, mColorSpecular.g, mColorSpecular.b,
                    mColorEmissive.r, mColorEmissive.g, mColorEmissive.b,
                    mColorReflective.r, mColorReflective.g, mColorReflective.b,

                    diff,
                    spec,
                    norm,
                    disp,

                    mUsedBy.size()
                    );

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

    void
    MaterialRuntime::drawGeometryPass
    (Camera* camera)
    {
        for (auto mesh : mUsedBy)
        {
            mesh->drawGeometryPassRuntimes(camera, mShaderHandle);
        }
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

        // Parameters
        mOpacity = matDef->getOpacity();
        mBumpScaling = matDef->getBumpScaling();
        mHardness = matDef->getHardness();
        mReflectivity = matDef->getReflectivity();
        mShininessStrength = matDef->getShininessStrength();
        mRefracti = matDef->getRefractionIndex();
        mIgnore = matDef->getIgnore();

        // Colours
        mColorDiffuse = matDef->getDiffuseColour().toGLM();
        mColorSpecular = matDef->getSpecularColour().toGLM();
        mColorAmbient = matDef->getAmbientColour().toGLM();
        mColorEmissive = matDef->getEmissiveColour().toGLM();
        mColorReflective = matDef->getReflectiveColour().toGLM();

        // Shaders & Textures
        auto shaderCache = mProjectRuntimeHandle->getShaderCache();
        auto textureCache = mProjectRuntimeHandle->getTextureCache();
        ShaderRuntime* shader = shaderCache->getRuntimeHandle(matDef->getShader());

        if (shader == nullptr)
        {
            LOG_ERROR("MaterialCache: Cannot create material {} with null shader", matDef->getNameAndUuidString());
            return false;
        }

        auto diffuse = textureCache->getRuntimeHandle(matDef->getDiffuseTexture());
        auto specular = textureCache->getRuntimeHandle(matDef->getSpecularTexture());
        auto normal = textureCache->getRuntimeHandle(matDef->getNormalTexture());
        auto displacement = textureCache->getRuntimeHandle(matDef->getDisplacementTexture());

        setDiffuseTextureHandle(diffuse);
        setSpecularTextureHandle(specular);
        setNormalTextureHandle(normal);
        setDisplacementTextureHandle(displacement);
        setShaderHandle(shader);
        shader->addMaterial(this);

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

    vec3 MaterialRuntime::getColorDiffuse() const
    {
        return mColorDiffuse;
    }

    void MaterialRuntime::setColorDiffuse(vec3 colorDiffuse)
    {
        mColorDiffuse = colorDiffuse;
    }

    vec3 MaterialRuntime::getColorAmbient() const
    {
        return mColorAmbient;
    }

    void MaterialRuntime::setColorAmbient(vec3 colorAmbient)
    {
        mColorAmbient = colorAmbient;
    }

    vec3 MaterialRuntime::getColorSpecular() const
    {
        return mColorSpecular;
    }

    void MaterialRuntime::setColorSpecular(vec3 colorSpecular)
    {
        mColorSpecular = colorSpecular;
    }

    vec3 MaterialRuntime::getColorEmissive() const
    {
        return mColorEmissive;
    }

    void MaterialRuntime::setColorEmissive(vec3 colorEmissive)
    {
        mColorEmissive = colorEmissive;
    }

    vec3 MaterialRuntime::getColorReflective() const
    {
        return mColorReflective;
    }

    void MaterialRuntime::setColorReflective(vec3 colorReflective)
    {
        mColorReflective = colorReflective;
    }

    float MaterialRuntime::getShininessStrength() const
    {
        return mShininessStrength;
    }

    void MaterialRuntime::setShininessStrength(float shininessStrength)
    {
        mShininessStrength = shininessStrength;
    }

    bool MaterialRuntime::getIgnore() const
    {
        return mIgnore;
    }

    void MaterialRuntime::setIgnore(bool ignore)
    {
        mIgnore = ignore;
    }

    TextureRuntime*
    MaterialRuntime::getDisplacementTextureHandle
    () const
    {
        return mDisplacementTextureHandle;
    }

    void
    MaterialRuntime::setDisplacementTextureHandle
    (TextureRuntime* displacementTexture)
    {
        mDisplacementTextureHandle = displacementTexture;
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
    MaterialRuntime::getSpecularTextureHandle
    () const
    {
        return mSpecularTextureHandle;
    }

    void
    MaterialRuntime::setSpecularTextureHandle
    (TextureRuntime* specularTexture)
    {
        mSpecularTextureHandle = specularTexture;
    }

    TextureRuntime*
    MaterialRuntime::getDiffuseTextureHandle
    () const
    {
        return mDiffuseTextureHandle;
    }

    void
    MaterialRuntime::setDiffuseTextureHandle
    (TextureRuntime* diffuseTexture)
    {
        mDiffuseTextureHandle = diffuseTexture;
    }


    void MaterialRuntime::pushNextTask()
    {
        auto taskQueue = mProjectRuntimeHandle->getTaskQueue();

        if (!mLoaded && !mLoadError && mLoadFromDefinitionTask->hasState(TASK_STATE_QUEUED))
        {
            taskQueue->pushTask(mLoadFromDefinitionTask);
        }
    }
}

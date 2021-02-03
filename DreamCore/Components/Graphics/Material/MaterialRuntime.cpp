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
#include "Components/Graphics/Shader/ShaderCache.h"
#include "Components/Graphics/Texture/TextureRuntime.h"
#include "Components/Graphics/Texture/TextureCache.h"
#include "Components/Graphics/Model/ModelMesh.h"
#include "Components/Graphics/Camera.h"
#include "Project/ProjectRuntime.h"

using std::unique_lock;

namespace octronic::dream
{
    MaterialRuntime::MaterialRuntime(MaterialDefinition* def, ProjectRuntime* rt)
        : SharedAssetRuntime("MaterialRuntime",def, rt),

          mColorDiffuse(vec3(0.0f)),
          mColorAmbient(vec3(0.0f)),
          mColorSpecular(vec3(0.0f)),
          mColorEmissive(vec3(0.0f)),
          mColorReflective(vec3(0.0f)),

          mDiffuseTexture(nullptr),
          mSpecularTexture(nullptr),
          mNormalTexture(nullptr),
          mDisplacementTexture(nullptr),

          mShader(nullptr)
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
       mUsedBy.push_back(mesh);
    }

    void
    MaterialRuntime::clearMeshes
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mUsedBy.clear();
    }

    size_t
    MaterialRuntime::countMeshes
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
       return mUsedBy.size();
    }

    bool
    MaterialRuntime::operator==
    (MaterialRuntime& other)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return getName() == other.getName();
    }

    void
    MaterialRuntime::debug
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        GLuint diff, spec, norm, disp;
        diff = (mDiffuseTexture  == nullptr ? 0 : mDiffuseTexture->getGLID());
        spec = (mSpecularTexture == nullptr ? 0 : mSpecularTexture->getGLID());
        norm = (mNormalTexture   == nullptr ? 0 : mNormalTexture->getGLID());
        disp = (mDisplacementTexture == nullptr ? 0 : mDisplacementTexture->getGLID());

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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
       for (auto mesh : mUsedBy)
       {
           mesh->drawGeometryPassRuntimes(camera, mShader);
       }
    }

    void
    MaterialRuntime::drawShadowPass
    (ShaderRuntime* shader)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
       for (auto mesh : mUsedBy)
       {
           mesh->drawShadowPassRuntimes(shader);
       }
    }

    bool
    MaterialRuntime::useDefinition
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        auto matDef = static_cast<MaterialDefinition*>(mDefinition);
        // Parameters
        mOpacity = matDef->getOpacity();
        mBumpScaling = matDef->getBumpScaling();
        mHardness = matDef->getHardness();
        mReflectivity = matDef->getReflectivity();
        mShininessStrength = matDef->getShininessStrength();
        mRefracti = matDef->getRefractionIndex();
        mIgnore = matDef->getIgnore();
        // Colours
        mColorDiffuse = rgbToVec3(matDef->getDiffuseColour());
        mColorSpecular = rgbToVec3(matDef->getSpecularColour());
        mColorAmbient = rgbToVec3(matDef->getAmbientColour());
        mColorEmissive = rgbToVec3(matDef->getEmissiveColour());
        mColorReflective = rgbToVec3(matDef->getReflectiveColour());

        // Shaders & Textures
        ShaderCache* shaderCache = mProjectRuntime->getShaderCache();
        TextureCache* textureCache = mProjectRuntime->getTextureCache();
        ShaderRuntime* shader = static_cast<ShaderRuntime*>(shaderCache->getRuntime(matDef->getShader()));

        if (shader == nullptr)
        {
            LOG_ERROR("MaterialCache: Cannot create material {} with null shader", matDef->getNameAndUuidString());
            return false;
        }

        TextureRuntime* diffuse = static_cast<TextureRuntime*>(textureCache->getRuntime(matDef->getDiffuseTexture()));
        TextureRuntime* specular = static_cast<TextureRuntime*>(textureCache->getRuntime(matDef->getSpecularTexture()));
        TextureRuntime* normal = static_cast<TextureRuntime*>(textureCache->getRuntime(matDef->getNormalTexture()));
        TextureRuntime* displacement = static_cast<TextureRuntime*>(textureCache->getRuntime(matDef->getDisplacementTexture()));

        setDiffuseTexture(diffuse);
        setSpecularTexture(specular);
        setNormalTexture(normal);
        setDisplacementTexture(displacement);
        setShader(shader);
        shader->addMaterial(this);

        return true;
    }

    ShaderRuntime*
    MaterialRuntime::getShader
    () const
    {
        return mShader;
    }

    void
    MaterialRuntime::setShader
    (ShaderRuntime* shader)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mShader = shader;
    }

    vec3 MaterialRuntime::getColorDiffuse() const
    {
        return mColorDiffuse;
    }

    void MaterialRuntime::setColorDiffuse(vec3 colorDiffuse)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mColorDiffuse = colorDiffuse;
    }

    vec3 MaterialRuntime::getColorAmbient() const
    {
        return mColorAmbient;
    }

    void MaterialRuntime::setColorAmbient(vec3 colorAmbient)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mColorAmbient = colorAmbient;
    }

    vec3 MaterialRuntime::getColorSpecular() const
    {
        return mColorSpecular;
    }

    void MaterialRuntime::setColorSpecular(vec3 colorSpecular)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mColorSpecular = colorSpecular;
    }

    vec3 MaterialRuntime::getColorEmissive() const
    {
        return mColorEmissive;
    }

    void MaterialRuntime::setColorEmissive(vec3 colorEmissive)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mColorEmissive = colorEmissive;
    }

    vec3 MaterialRuntime::getColorReflective() const
    {
        return mColorReflective;
    }

    void MaterialRuntime::setColorReflective(vec3 colorReflective)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mColorReflective = colorReflective;
    }

    float MaterialRuntime::getShininessStrength() const
    {
        return mShininessStrength;
    }

    void MaterialRuntime::setShininessStrength(float shininessStrength)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mShininessStrength = shininessStrength;
    }

    bool MaterialRuntime::getIgnore() const
    {
        return mIgnore;
    }

    void MaterialRuntime::setIgnore(bool ignore)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mIgnore = ignore;
    }

    vec3
    MaterialRuntime::rgbToVec3
    (RGB color)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return vec3(color.r,color.g,color.b);
    }

    TextureRuntime*
    MaterialRuntime::getDisplacementTexture
    () const
    {
        return mDisplacementTexture;
    }

    void
    MaterialRuntime::setDisplacementTexture
    (TextureRuntime* displacementTexture)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mDisplacementTexture = displacementTexture;
    }

    TextureRuntime*
    MaterialRuntime::getNormalTexture
    () const
    {
        return mNormalTexture;
    }

    void
    MaterialRuntime::setNormalTexture
    (TextureRuntime* normalTexture)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mNormalTexture = normalTexture;
    }

    TextureRuntime*
    MaterialRuntime::getSpecularTexture
    () const
    {
        return mSpecularTexture;
    }

    void
    MaterialRuntime::setSpecularTexture
    (TextureRuntime* specularTexture)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mSpecularTexture = specularTexture;
    }

    TextureRuntime*
    MaterialRuntime::getDiffuseTexture
    () const
    {
        return mDiffuseTexture;
    }

    void
    MaterialRuntime::setDiffuseTexture
    (TextureRuntime* diffuseTexture)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mDiffuseTexture = diffuseTexture;
    }
}

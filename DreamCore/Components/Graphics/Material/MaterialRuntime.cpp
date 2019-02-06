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
#include "../Shader/ShaderRuntime.h"
#include "../Texture/TextureRuntime.h"
#include "../Model/ModelMesh.h"
#include "MaterialDefinition.h"
#include "../Camera.h"

namespace Dream
{
    MaterialRuntime::MaterialRuntime(MaterialDefinition* def, ProjectRuntime* rt)
        : SharedAssetRuntime(def, rt),

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
#ifdef DREAM_LOG
        setLogClassName("MaterialRuntime");
        auto log = getLog();
        log->trace("Constructing");
#endif
    }

    MaterialRuntime::~MaterialRuntime()
    {
#ifdef DREAM_LOG
        auto log = getLog();
        log->trace("Destructing");
#endif
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

#ifdef DREAM_LOG
    void
    MaterialRuntime::debug
    ()
    {
        GLuint diff, spec, norm, disp;
        diff = (mDiffuseTexture  == nullptr ? 0 : mDiffuseTexture->getGLID());
        spec = (mSpecularTexture == nullptr ? 0 : mSpecularTexture->getGLID());
        norm = (mNormalTexture   == nullptr ? 0 : mNormalTexture->getGLID());
        disp = (mDisplacementTexture == nullptr ? 0 : mDisplacementTexture->getGLID());

        auto log = getLog();
        log->trace(
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
        auto log = getLog();
        log->debug("\tMeshes for material {} : {}",getName(),mUsedBy.size());
        for (auto mesh : mUsedBy)
        {
            log->debug("\t\t{}", mesh->getName());
            mesh->logRuntimes();
        }
    }
#endif

    void
    MaterialRuntime::drawGeometryPass
    (Camera* camera)
    {
       for (auto mesh : mUsedBy)
       {
           mesh->drawGeometryPassRuntimes(camera, mShader);
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
    MaterialRuntime::useDefinition
    ()
    {
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
        mShader = shader;
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

    vec3
    MaterialRuntime::rgbToVec3
    (RGB color)
    {
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
        mDiffuseTexture = diffuseTexture;
    }
}

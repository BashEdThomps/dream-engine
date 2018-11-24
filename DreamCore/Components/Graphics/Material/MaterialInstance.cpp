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


#include "MaterialInstance.h"
#include "../Shader/ShaderInstance.h"
#include "../Texture/TextureInstance.h"
#include "../Model/ModelMesh.h"
#include "MaterialDefinition.h"
#include "../Camera.h"

namespace Dream
{
    MaterialInstance::MaterialInstance(MaterialDefinition* def, SceneObjectRuntime* rt)
        : IAssetInstance(def, rt),
          mDiffuseTexture(nullptr),
          mSpecularTexture(nullptr),
          mNormalTexture(nullptr),
          mDisplacementTexture(nullptr),
          mShader(nullptr)
    {
        setLogClassName("MaterialInstance");
        auto log = getLog();
        log->trace("Constructing");
    }

    MaterialInstance::~MaterialInstance()
    {
        auto log = getLog();
        log->trace("Destructing");
    }

    void
    MaterialInstance::addMesh
    (ModelMesh* mesh)
    {
       mUsedBy.push_back(mesh);
    }

    void
    MaterialInstance::clearMeshes
    ()
    {
        mUsedBy.clear();
    }

    size_t
    MaterialInstance::countMeshes
    ()
    {
       return mUsedBy.size();
    }

    bool
    MaterialInstance::operator==
    (MaterialInstance& other)
    {
        return getName().compare(other.getName()) == 0;
    }

    void
    MaterialInstance::debug
    ()
    {
        GLuint diff, spec, norm, disp;
        diff = (mDiffuseTexture  == nullptr ? 0 : mDiffuseTexture->getGLID());
        spec = (mSpecularTexture == nullptr ? 0 : mSpecularTexture->getGLID());
        norm = (mNormalTexture   == nullptr ? 0 : mNormalTexture->getGLID());
        disp = (mDisplacementTexture == nullptr ? 0 : mDisplacementTexture->getGLID());

        auto log = getLog();
        log->debug(
            "Material Parameters\n"
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
    MaterialInstance::logMeshes
    ()
    {
        auto log = getLog();
        log->debug("\tMeshes for material {} : {}",getName(),mUsedBy.size());
        for (auto mesh : mUsedBy)
        {
            log->debug("\t\t{}", mesh->getName());
            mesh->logInstances();
        }
    }

    void
    MaterialInstance::drawGeometryPass
    (Camera* camera)
    {

       for (auto mesh : mUsedBy)
       {
           mesh->drawGeometryPassInstances(camera, mShader);
       }
    }

    void
    MaterialInstance::drawShadowPass
    (ShaderInstance* shader)
    {
       for (auto mesh : mUsedBy)
       {
           mesh->drawShadowPassInstances(shader);
       }
    }

    bool
    MaterialInstance::load
    (string)
    {
        auto matDef = dynamic_cast<MaterialDefinition*>(mDefinition);
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

    ShaderInstance*
    MaterialInstance::getShader
    () const
    {
        return mShader;
    }

    void
    MaterialInstance::setShader
    (ShaderInstance* shader)
    {
        mShader = shader;
    }

    vec3 MaterialInstance::getColorDiffuse() const
    {
        return mColorDiffuse;
    }

    void MaterialInstance::setColorDiffuse(vec3 colorDiffuse)
    {
        mColorDiffuse = colorDiffuse;
    }

    vec3 MaterialInstance::getColorAmbient() const
    {
        return mColorAmbient;
    }

    void MaterialInstance::setColorAmbient(vec3 colorAmbient)
    {
        mColorAmbient = colorAmbient;
    }

    vec3 MaterialInstance::getColorSpecular() const
    {
        return mColorSpecular;
    }

    void MaterialInstance::setColorSpecular(vec3 colorSpecular)
    {
        mColorSpecular = colorSpecular;
    }

    vec3 MaterialInstance::getColorEmissive() const
    {
        return mColorEmissive;
    }

    void MaterialInstance::setColorEmissive(vec3 colorEmissive)
    {
        mColorEmissive = colorEmissive;
    }

    vec3 MaterialInstance::getColorReflective() const
    {
        return mColorReflective;
    }

    void MaterialInstance::setColorReflective(vec3 colorReflective)
    {
        mColorReflective = colorReflective;
    }

    float MaterialInstance::getShininessStrength() const
    {
        return mShininessStrength;
    }

    void MaterialInstance::setShininessStrength(float shininessStrength)
    {
        mShininessStrength = shininessStrength;
    }

    bool MaterialInstance::getIgnore() const
    {
        return mIgnore;
    }

    void MaterialInstance::setIgnore(bool ignore)
    {
        mIgnore = ignore;
    }

    vec3
    MaterialInstance::rgbToVec3
    (RGB color)
    {
        return vec3(color.r,color.g,color.b);
    }

    TextureInstance*
    MaterialInstance::getDisplacementTexture
    () const
    {
        return mDisplacementTexture;
    }

    void
    MaterialInstance::setDisplacementTexture
    (TextureInstance* displacementTexture)
    {
        mDisplacementTexture = displacementTexture;
    }

    TextureInstance*
    MaterialInstance::getNormalTexture
    () const
    {
        return mNormalTexture;
    }

    void
    MaterialInstance::setNormalTexture
    (TextureInstance* normalTexture)
    {
        mNormalTexture = normalTexture;
    }

    TextureInstance*
    MaterialInstance::getSpecularTexture
    () const
    {
        return mSpecularTexture;
    }

    void
    MaterialInstance::setSpecularTexture
    (TextureInstance* specularTexture)
    {
        mSpecularTexture = specularTexture;
    }

    TextureInstance*
    MaterialInstance::getDiffuseTexture
    () const
    {
        return mDiffuseTexture;
    }

    void
    MaterialInstance::setDiffuseTexture
    (TextureInstance* diffuseTexture)
    {
        mDiffuseTexture = diffuseTexture;
    }
}

#include "Material.h"
#include "../ModelMesh.h"

namespace Dream
{
    Material::Material()
        : DreamObject ("Material"),
          mDiffuseTexture(nullptr),
          mSpecularTexture(nullptr),
          mNormalTexture(nullptr)
    {
        auto log = getLog();
        log->trace("Constructing");
    }

    Material::~Material()
    {
        auto log = getLog();
        log->trace("Destructing");
    }

    void
    Material::addMesh
    (const shared_ptr<ModelMesh> mesh)
    {
       mUsedBy.push_back(mesh);
    }

    void
    Material::clearMeshes
    ()
    {
       mUsedBy.clear();
    }

    bool
    Material::operator==
    (const Material& other)
    {
        return mName.operator==(other.mName);
    }

    void
    Material::debug
    ()
    {
        GLuint diff, spec, norm, disp;
        diff = (mDiffuseTexture  == nullptr ? 0 : mDiffuseTexture->id);
        spec = (mSpecularTexture == nullptr ? 0 : mSpecularTexture->id);
        norm = (mNormalTexture   == nullptr ? 0 : mNormalTexture->id);
        disp = (mDisplacementTexture == nullptr ? 0 : mDisplacementTexture->id);

        auto log = getLog();
        log->debug(
            "Material Parameters\n"
            "Name....................{}\n"
            "TwoSided................{}\n"
            "ShadingModel............{}\n"
            "Wireframe...............{}\n"
            "BlendFunc...............{}\n"
            "Opacity.................{}\n"
            "BumpScaling.............{}\n"
            "Hardness................{}\n"
            "Reflectivity............{}\n"
            "ShininessStrength.......{}\n"
            "RefractI................{}\n"

            "DiffuseColour...........({},{},{})\n"
            "AmbientColour...........({},{},{})\n"
            "SpecularColour..........({},{},{})\n"
            "EmissiveColour..........({},{},{})\n"
            "TransparentColour.......({},{},{})\n"
            "ReflectiveColour........({},{},{})\n"

            "DiffuseTexture..........{}\n"
            "SpecularTexture.........{}\n"
            "NormalTexture...........{}\n"
            "DisplacementTexture.....{}\n"
            "Meshes..................{}",
            mName.C_Str(),
            mTwoSided,
            mShadingModel,
            mEnableWireframe,
            mBlendFunc,
            mOpacity,
            mBumpScaling,
            mHardness,
            mReflectivity,
            mShininessStrength,
            mRefracti,
            mColorDiffuse.r, mColorDiffuse.g, mColorDiffuse.b,
            mColorAmbient.r, mColorAmbient.g, mColorAmbient.b,
            mColorSpecular.r, mColorSpecular.g, mColorSpecular.b,
            mColorEmissive.r, mColorEmissive.g, mColorEmissive.b,
            mColorTransparent.r, mColorTransparent.g, mColorTransparent.b,
            mColorReflective.r, mColorReflective.g, mColorReflective.b,
            diff,
            spec,
            norm,
            disp,
            mUsedBy.size()
                    );
    }

    void
    Material::logMeshes
    ()
    {
        auto log = getLog();
        log->debug("\tMeshes for material {} : {}",mName.C_Str(),mUsedBy.size());
        for (auto mesh : mUsedBy)
        {
            log->debug("\t\t{}", mesh->getName());
            mesh->logInstances();
        }
    }

    void
    Material::draw
    (ShaderInstance* shader)
    {
       for (auto mesh : mUsedBy)
       {
           mesh->drawInstances(shader);
       }
    }
}

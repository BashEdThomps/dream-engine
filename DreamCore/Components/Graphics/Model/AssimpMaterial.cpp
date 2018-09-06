#include "AssimpMaterial.h"
#include "AssimpMesh.h"

namespace Dream
{
    AssimpMaterial::AssimpMaterial()
        : DreamObject ("AssimpMaterial"),
          mDiffuseTexture(nullptr),
          mSpecularTexture(nullptr),
          mNormalTexture(nullptr)
    {
        auto log = getLog();
        log->trace("Constructing");
    }

    AssimpMaterial::~AssimpMaterial()
    {
        auto log = getLog();
        log->trace("Destructing");
    }

    bool AssimpMaterial::operator==(const AssimpMaterial& other)
    {
        return mName.operator==(other.mName);
    }

    void AssimpMaterial::debug()
    {
        GLuint diff, spec, norm;
        diff = (mDiffuseTexture  == nullptr) ? 0 : mDiffuseTexture->id;
        spec = (mSpecularTexture == nullptr) ? 0 : mSpecularTexture->id;
        norm = (mNormalTexture   == nullptr) ? 0 : mNormalTexture->id;

        auto log = getLog();
        log->info(
            "Material Parameters\n"
            "Name....................{}\n"
            "TwoSided................{}\n"
            "ShadingModel............{}\n"
            "Wireframe...............{}\n"
            "BlendFunc...............{}\n"
            "Opacity.................{}\n"
            "BumpScaling.............{}\n"
            "Hardness...............{}\n"
            "Reflectivity............{}\n"
            "ShininessStrength.......{}\n"
            "RefractI................{}\n"
            "DiffuseColour...........({},{},{})\n"
            "AmbientColour...........({},{},{})\n"
            "SpecularColour..........({},{},{})\n"
            "EmissiveColour..........({},{},{})\n"
            "ReflectiveColour........({},{},{})\n"
            "DiffuseTexture..........{}\n"
            "SpecularTexture.........{}\n"
            "NormalTexture...........{}\n",
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
            diff,spec,norm
        );

    }
}

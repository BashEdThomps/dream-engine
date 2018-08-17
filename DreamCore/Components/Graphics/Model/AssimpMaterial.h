#pragma once

#include <assimp/types.h>

namespace Dream
{
    class AssimpMaterial
    {
    public:
        aiString mName;
        int mTwoSided;
        int mShadingModel;
        int mEnableWireframe;
        int mBlendFunc;
        ai_real mOpacity;
        ai_real mBumpScaling;
        ai_real mShininess;
        ai_real mReflectivity;
        ai_real mShininessStrength;
        ai_real mRefracti;
        aiColor4D mColorDiffuse;
        aiColor4D mColorAmbient;
        aiColor4D mColorSpecular;
        aiColor4D mColorEmissive;
        aiColor4D mColorTransparent;
        aiColor4D mColorReflective;
    };
}

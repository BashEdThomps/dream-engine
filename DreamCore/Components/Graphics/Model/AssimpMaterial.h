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
        aiString mBlendFunc;
        ai_real mOpacity;
        float mBumpScaling;
        ai_real mShininess;
        ai_real mReflectivity;
        ai_real mShininessStrength;
        ai_real mRefracti;
        aiColor3D mColorDiffuse;
        aiColor3D mColorAmbient;
        aiColor3D mColorSpecular;
        aiColor3D mColorEmissive;
        aiColor3D mColorTransparent;
        aiColor3D mColorReflective;
    };
}

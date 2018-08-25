#pragma once

#include <assimp/types.h>
#include "Texture.h"
#include <vector>
#include <memory>
#include "../Vertex.h"

using namespace std;

namespace Dream
{
    class AssimpMesh;
    class AssimpMaterial
    {
    public:
        aiString mName;
        int mTwoSided = -1;
        int mShadingModel = -1;
        int mEnableWireframe = -1;
        int mBlendFunc = -1;
        ai_real mOpacity = 0.0f;
        ai_real mBumpScaling = 0.0f;
        ai_real mShininess = 0.0f;
        ai_real mReflectivity = 0.0f;
        ai_real mShininessStrength = 0.0f;
        ai_real mRefracti = 0.0f;
        aiColor4D mColorDiffuse;
        aiColor4D mColorAmbient;
        aiColor4D mColorSpecular;
        aiColor4D mColorEmissive;
        aiColor4D mColorTransparent;
        aiColor4D mColorReflective;
        shared_ptr<Texture> mDiffuseTexture;
        shared_ptr<Texture> mSpecularTexture;
        shared_ptr<Texture> mNormalTexture;
        bool operator==(const AssimpMaterial& other);
    };
}

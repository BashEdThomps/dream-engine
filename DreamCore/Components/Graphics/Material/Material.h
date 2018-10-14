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


#pragma once

#include <assimp/types.h>
#include "../Texture/Texture.h"
#include <vector>
#include <memory>
#include "../../../Common/DreamObject.h"

using namespace std;

namespace Dream
{
    class ModelMesh;
    class ShaderInstance;

    class Material : public DreamObject
    {
    public:
        Material();
        ~Material() override;
        void addMesh(const shared_ptr<ModelMesh> mesh);
        void clearMeshes();

        aiString mName;
        int mTwoSided = -1;
        int mShadingModel = -1;
        int mEnableWireframe = -1;
        int mBlendFunc = -1;
        ai_real mOpacity = 0.0f;
        ai_real mBumpScaling = 0.0f;
        ai_real mHardness = 0.0f;
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
        shared_ptr<Texture> mDisplacementTexture;
        bool operator==(const Material& other);
        void debug();
        vector<const shared_ptr<ModelMesh>> mUsedBy;
        void logMeshes();
        void draw(ShaderInstance*);
    };
}

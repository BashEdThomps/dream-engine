/*
 * TextureManager.h
 *
 * Created: 30/11/2016 2016 by Ashley
 *
 * Copyright 2016 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#pragma once

#include <iostream>
#include <vector>
#include <map>

#include <SOIL/SOIL.h>
#include <GL/glew.h>

#include "Texture.h"
#include "../../../Common/Constants.h"
#include "../../../Common/DreamObject.h"
#include "AssimpMaterial.h"
#include <assimp/material.h>


using namespace std;

namespace Dream
{
    class AssimpMesh;
    class MaterialCache : public DreamObject
    {
        vector<shared_ptr<Texture>> mTextureCache;
        vector<shared_ptr<AssimpMaterial>> mMaterialCache;
    public:
        MaterialCache();
        ~MaterialCache();

        vector<shared_ptr<Texture>>& getTextureCache();
        vector<shared_ptr<AssimpMaterial>>& getMaterialCache();
        shared_ptr<Texture> loadTextureFromFile(const char*, const char*, const aiTextureType);
        shared_ptr<AssimpMaterial> newAssimpMaterial();
        shared_ptr<AssimpMaterial> getMaterialByName(aiString name);
        void addMaterialToCache(shared_ptr<AssimpMaterial> mat);
        void flushRawTextureImageData();
    };

} // End Dream


/*
 * AssimpCache.cpp
 *
 * Created: 20 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
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

#include "AssimpCache.h"

#include "../../../Common/Constants.h"

#include <iostream>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

using std::pair;
using std::cout;
using std::endl;

namespace Dream
{
    AssimpCache::AssimpCache
    ()
    {
            cout << "AssimpCache: Contructing" << endl;
    }

    AssimpCache::~AssimpCache
    ()
    {
            cout << "AssimpCache: Destructing" << endl;
        for (auto imp : mCache)
        {
            if (imp.second != nullptr)
            {
                delete imp.second;
            }
        }
    }

    Importer*
    AssimpCache::getModelFromCache
    (string path)
    {
        for (pair<string,Importer*> it : mCache)
        {
            if (it.first.compare(path) == 0)
            {
                    cout << "AssimpCache: Found cached scene for " << path << endl;
                return it.second;
            }
        }

          cout << "AssimpCache: Loading " << path << " from disk" << endl;

        Importer* importer = new Importer();
        importer->ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        const aiScene* scene = importer->GetScene();
        if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            cerr << "AssimpCache: Error " << importer->GetErrorString() << endl;
            return nullptr;
        }

        mCache.insert(pair<string,Importer*>(path,importer));
        return importer;
    }
}

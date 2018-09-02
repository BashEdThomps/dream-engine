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

namespace Dream
{
    AssimpCache::AssimpCache
    ()
        :ILoggable ("AssimpCache")
    {
        auto log = getLog();
            log->info("Contructing" );
    }

    AssimpCache::~AssimpCache
    ()
    {
        auto log = getLog();
            log->info("Destructing" );
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
        auto log = getLog();
        for (pair<string,Importer*> it : mCache)
        {
            if (it.first.compare(path) == 0)
            {
                    log->info("Found cached scene for {}", path );
                return it.second;
            }
        }

          log->info("Loading {} from disk",  path);

        Importer* importer = new Importer();
        importer->ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        const aiScene* scene = importer->GetScene();
        if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            log->error( "Error {}" ,importer->GetErrorString() );
            return nullptr;
        }

        mCache.insert(pair<string,Importer*>(path,importer));
        return importer;
    }
}

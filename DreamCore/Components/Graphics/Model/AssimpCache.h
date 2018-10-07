/*
 * AssimpCache.h
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
#pragma once

#include <string>
#include <vector>
#include <assimp/Importer.hpp>

#include "../../../Common/DreamObject.h"
#include "AssimpMesh.h"

using std::string;
using std::vector;

namespace Dream
{
    class MaterialCache;
    class ModelDefinition;
    class SceneObjectRuntime;

    class AssimpCache : public DreamObject
    {
    private:
        vector<shared_ptr<AssimpModelInstance>> mCache;
    public:
        AssimpCache(MaterialCache*);
        ~AssimpCache();
        shared_ptr<AssimpModelInstance> getModelFromCache(string, ModelDefinition*,SceneObjectRuntime*);
    protected:
        MaterialCache* mMaterialCacheHandle;
    };
}

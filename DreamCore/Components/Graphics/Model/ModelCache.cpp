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

#include "ModelCache.h"
#include "ModelRuntime.h"
#include "ModelDefinition.h"
#include "Project/ProjectRuntime.h"

namespace Dream
{
    ModelCache::ModelCache
    (ProjectRuntime* rt, ShaderCache* shaderCache, MaterialCache* matCache)
        : Cache(rt),
          mShaderCache(shaderCache),
          mMaterialCache(matCache)
    {
        LOG_DEBUG("ModelCache: Contructing" );
    }

    ModelCache::~ModelCache
    ()
    {
        LOG_DEBUG("ModelCache: Destructing" );
    }

    SharedAssetRuntime*
    ModelCache::loadRuntime
    (AssetDefinition* def)
    {
        LOG_DEBUG("ModelCache: Loading {} from disk",  def->getUuid());
        auto model = new ModelRuntime(mShaderCache, mMaterialCache,def,mProjectRuntime);
        if (model->useDefinition())
        {
            mRuntimes.push_back(model);
        }
        else
        {
            LOG_ERROR("ModelCache: Unable to create runtime for model {}", def->getNameAndUuidString());
            delete model;
            model = nullptr;
        }
        return model;
    }
}

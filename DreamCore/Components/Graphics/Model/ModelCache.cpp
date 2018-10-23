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
#include "ModelInstance.h"
#include "ModelDefinition.h"
#include "../../../Project/ProjectRuntime.h"

namespace Dream
{
    ModelCache::ModelCache
    (ProjectRuntime* rt, ShaderCache* shaderCache, MaterialCache* matCache)
        :ICache(rt),
          mShaderCacheHandle(shaderCache),
          mMaterialCacheHandle(matCache)
    {
        setLogClassName("ModelCache");
        auto log = getLog();
        log->debug("Contructing" );
    }

    ModelCache::~ModelCache
    ()
    {
        auto log = getLog();
        log->debug("Destructing" );
    }

    IAssetInstance*
    ModelCache::loadInstance
    (IAssetDefinition* def)
    {
        auto log = getLog();
        log->debug("Loading {} from disk",  def->getUuid());
        auto model = new ModelInstance(mShaderCacheHandle, mMaterialCacheHandle,def);
        model->load(mProjectRuntime->getProjectPath());
        mInstances.push_back(model);
        return model;
    }
}

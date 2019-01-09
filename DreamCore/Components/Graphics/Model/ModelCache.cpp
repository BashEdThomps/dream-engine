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
#include "../../../Project/ProjectRuntime.h"

namespace Dream
{
    ModelCache::ModelCache
    (ProjectRuntime* rt, ShaderCache* shaderCache, MaterialCache* matCache)
        : Cache(rt),
          mShaderCache(shaderCache),
          mMaterialCache(matCache)
    {
#ifdef DREAM_LOG
        setLogClassName("ModelCache");
        auto log = getLog();
        log->debug("Contructing" );
#endif
    }

    ModelCache::~ModelCache
    ()
    {
#ifdef DREAM_LOG
        auto log = getLog();
        log->debug("Destructing" );
#endif
    }

    SharedAssetRuntime*
    ModelCache::loadInstance
    (AssetDefinition* def)
    {
#ifdef DREAM_LOG
        auto log = getLog();
        log->debug("Loading {} from disk",  def->getUuid());
#endif
        auto model = new ModelRuntime(mShaderCache, mMaterialCache,def,mProjectRuntime);
        if (model->useDefinition())
        {
            mInstances.push_back(model);
        }
        else
        {
#ifdef DREAM_LOG
            log->error("Unable to create runtime for model {}", def->getNameAndUuidString());
#endif
            delete model;
            model = nullptr;
        }
        return model;
    }
}

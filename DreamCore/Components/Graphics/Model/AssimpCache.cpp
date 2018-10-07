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
#include <iostream>
#include "AssimpModelInstance.h"
#include "ModelDefinition.h"
#include "../../../Common/Constants.h"

using std::pair;

namespace Dream
{
    AssimpCache::AssimpCache
    (MaterialCache* matCache)
        :DreamObject ("AssimpCache"),
          mMaterialCacheHandle(matCache)
    {
        auto log = getLog();
        log->debug("Contructing" );
    }

    AssimpCache::~AssimpCache
    ()
    {
        auto log = getLog();
        log->debug("Destructing" );
        mCache.clear();
    }

    shared_ptr<AssimpModelInstance>
    AssimpCache::getModelFromCache
    (string projectPath, ModelDefinition* def, SceneObjectRuntime* rt)
    {
        auto log = getLog();
        for (auto& ami : mCache)
        {
            if (ami->getUuid().compare(def->getUuid()) == 0)
            {
                log->debug("Found cached model for {}", def->getUuid());
                return ami;
            }
        }

        log->debug("Loading {} from disk",  def->getUuid());
        auto model = make_shared<AssimpModelInstance>(mMaterialCacheHandle,def,rt);
        model->load(projectPath);
        mCache.push_back(model);
        return model;
    }
}

/*
 * ShaderCache
 *
 * Created: 16/12/2016 2016 by Ashley
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
#include "ShaderCache.h"
#include "ShaderDefinition.h"
#include "ShaderInstance.h"
#include "../../../Project/ProjectRuntime.h"

namespace Dream
{
    ShaderCache::ShaderCache
    (ProjectRuntime* rt)
        : ICache(rt)
    {
        setLogClassName("ShaderCache");
        auto log = getLog();
        log->trace( "Constructing" );
    }

    ShaderCache::~ShaderCache
    ()
    {
        auto log = getLog();
        log->trace( "Destructing" );
    }

    IAssetInstance*
    ShaderCache::loadInstance
    (IAssetDefinition* def)
    {
        auto log = getLog();
        auto shaderInstance = new ShaderInstance(dynamic_cast<ShaderDefinition*>(def));

		if (shaderInstance->load(mProjectRuntime->getProjectPath()))
		{
			log->error("Error while loading shader {}", def->getUuid());
		}
		mInstances.push_back(shaderInstance);
		return shaderInstance;
    }

    void
    ShaderCache::logShaders
    ()
    {
        auto log = getLog();
        log->debug("Contents of shader cache");
        for (auto instance : mInstances)
        {
            auto shader = dynamic_cast<ShaderInstance*>(instance);
            log->debug("{}",shader->getNameAndUuidString());
            shader->logMaterials();
        }
    }

    void
    ShaderCache::draw
    (
        mat4 viewMatrix,
        mat4 projectionMatrix,
        vec3 viewPos
    )
    {
        for (auto instance : mInstances)
        {
            auto shader = dynamic_cast<ShaderInstance*>(instance);
            if (shader->countMaterials() == 0) continue;

            shader->use();
            shader->setViewMatrix(viewMatrix);
            shader->setProjectionMatrix(projectionMatrix);
            shader->setViewerPosition(viewPos);
            shader->draw();
        }
    }
} // End of Dream



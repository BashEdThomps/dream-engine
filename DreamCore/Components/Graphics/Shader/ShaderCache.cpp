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
#include "../Camera.h"

namespace Dream
{
    ShaderCache::ShaderCache
    (ProjectRuntime* rt)
        : Cache(rt)
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

    SharedAssetInstance*
    ShaderCache::loadInstance
    (AssetDefinition* def)
    {
        auto log = getLog();
        auto shaderInstance = new ShaderInstance(dynamic_cast<ShaderDefinition*>(def), mProjectRuntime);

        if (!shaderInstance->load())
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
    ShaderCache::drawGeometryPass
    (Camera* camera)
    {
        for (auto instance : mInstances)
        {
            auto shader = dynamic_cast<ShaderInstance*>(instance);
            if (shader->countMaterials() == 0) continue;
            shader->use();
            shader->setViewMatrix(camera->getViewMatrix());
            shader->setProjectionMatrix(camera->getProjectionMatrix());
            shader->setViewerPosition(camera->getTranslation());
            shader->drawGeometryPass(camera);
        }
    }

    void
    ShaderCache::drawShadowPass
    (
        mat4 matrix, ShaderInstance* shader
    )
    {
        shader->use();
        auto lsUniform = shader->getUniformLocation("lightSpaceMatrix");
        glUniformMatrix4fv(lsUniform,1,GL_FALSE,glm::value_ptr(matrix));
        for (auto instance : mInstances)
        {
            auto s = dynamic_cast<ShaderInstance*>(instance);
            s->drawShadowPass(shader);
        }
    }
} // End of Dream



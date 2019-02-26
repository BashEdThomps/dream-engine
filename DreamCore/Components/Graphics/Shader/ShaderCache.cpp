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
#include "ShaderRuntime.h"
#include "../../../Project/ProjectRuntime.h"
#include "../Camera.h"

namespace Dream
{
    ShaderCache::ShaderCache
    (ProjectRuntime* rt)
        : Cache(rt)
    {
#ifdef DREAM_LOG
        setLogClassName("ShaderCache");
        auto log = getLog();
        log->trace( "Constructing" );
#endif
    }

    ShaderCache::~ShaderCache
    ()
    {
#ifdef DREAM_LOG
        auto log = getLog();
        log->trace( "Destructing" );
#endif
    }

    SharedAssetRuntime*
    ShaderCache::loadRuntime
    (AssetDefinition* def)
    {
        auto shaderRuntime = new ShaderRuntime(static_cast<ShaderDefinition*>(def), mProjectRuntime);

        if (!shaderRuntime->useDefinition())
        {
#ifdef DREAM_LOG
            getLog()->error("Error while loading shader {}", def->getUuid());
#endif
        }
        mRuntimes.push_back(shaderRuntime);
        return shaderRuntime;
    }

#ifdef DREAM_LOG
    void
    ShaderCache::logShaders
    ()
    {
        auto log = getLog();
        log->debug("Contents of shader cache");
        for (auto runtime : mRuntimes)
        {
            auto shader = static_cast<ShaderRuntime*>(runtime);
            log->debug("{}",shader->getNameAndUuidString());
            shader->logMaterials();
        }
    }
#endif

    void
    ShaderCache::drawGeometryPass
    (Camera* camera)
    {
        for (auto runtime : mRuntimes)
        {
            auto shader = static_cast<ShaderRuntime*>(runtime);
            if (shader->countMaterials() == 0) continue;
            if(shader->use())
            {
                shader->setViewMatrix(camera->getViewMatrix());
                shader->setProjectionMatrix(camera->getProjectionMatrix());
                shader->setViewerPosition(camera->getTranslation());
                shader->drawGeometryPass(camera);
            }
        }
    }

    void
    ShaderCache::drawShadowPass
    (mat4 matrix, ShaderRuntime* shader)
    {
        if(shader->use())
        {
            auto lsUniform = shader->getUniformLocation("lightSpaceMatrix");
            glUniformMatrix4fv(lsUniform,1,GL_FALSE,glm::value_ptr(matrix));
            for (auto runtime : mRuntimes)
            {
                auto s = static_cast<ShaderRuntime*>(runtime);
                s->drawShadowPass(shader);
            }
        }
    }
}



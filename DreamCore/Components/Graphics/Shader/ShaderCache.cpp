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

namespace Dream
{


    ShaderCache::ShaderCache
    ()
        :DreamObject ("ShaderCache")
    {
        auto log = getLog();
        log->trace( "Constructing" );
    }

    ShaderCache::~ShaderCache
    ()
    {
        auto log = getLog();
        log->trace( "Destructing" );
        mCache.clear();
    }

    shared_ptr<ShaderInstance>
    ShaderCache::getShaderByUuid
    (string uuid)
    {
        auto log = getLog();
        for(auto it : mCache)
        {
            if (it.first.compare(uuid) == 0)
            {
                log->debug( "Found Shader " , uuid );
                return it.second;
            }
        }
        return nullptr;
    }

    shared_ptr<ShaderInstance>
    ShaderCache::getShaderFromCache
    (string projectPath, ShaderDefinition* definition, SceneObjectRuntime* runt)
    {
        auto log = getLog();
        auto uuid = definition->getUuid();
        auto shaderInstance = getShaderByUuid(uuid);
        if (shaderInstance != nullptr)
        {
            log->debug("Found shader {} in cache", uuid);
            return shaderInstance;
        }
        log->debug("Loading new shader {}, path: {}",uuid,projectPath);
        shaderInstance = make_shared<ShaderInstance>(definition,runt);
        if(shaderInstance->load(projectPath))
        {
            mCache.insert
            (
                pair<string,shared_ptr<ShaderInstance>>
                (
                    uuid,
                    shaderInstance
                )
            );
            return shaderInstance;
        }
        else
        {
            log->error("Error while loading shader {}", uuid);
        }
        return nullptr;
    }

    void
    ShaderCache::logShaders
    ()
    {
        auto log = getLog();
        log->debug("Contents of shader cache");
        for (auto shaderPair : mCache)
        {
            log->debug("{}",shaderPair.second->getNameAndUuidString());
            shaderPair.second->logMaterials();
        }
    }

    void
    ShaderCache::draw
    (
        mat4 viewMatrix,
        mat4 projectionMatrix,
        vec3 viewPos,
        vector<LightInstance*> lightQueue
    )
    {
        for (auto shaderPair : mCache)
        {
            auto shader = shaderPair.second;
            shader->use();
            shader->setViewMatrix(viewMatrix);
            shader->setProjectionMatrix(projectionMatrix);
            shader->setViewerPosition(viewPos);
            shader->bindLightQueue(lightQueue);
            shader->draw();
        }
    }
} // End of Dream

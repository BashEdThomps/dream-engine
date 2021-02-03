/*
 * TextureCache.cpp
 *
 * Created: 30/11/2016 2016 by Ashley
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

#include "MaterialCache.h"

#include "MaterialDefinition.h"
#include "MaterialRuntime.h"
#include "Common/Logger.h"
#include "Components/Graphics/Texture/TextureDefinition.h"
#include "Components/Graphics/Texture/TextureRuntime.h"
#include "Components/Graphics/Texture/TextureCache.h"
#include "Components/Graphics/Shader/ShaderRuntime.h"
#include "Components/Graphics/Shader/ShaderCache.h"
#include "Project/Project.h"
#include "Components/Storage/ProjectDirectory.h"
#include "Project/ProjectRuntime.h"

using std::unique_lock;

namespace octronic::dream
{
    MaterialCache::MaterialCache
    (ProjectRuntime* parent, ShaderCache* shaderCache, TextureCache* textureCache)
        : Cache("MaterialCache",parent),
          mShaderCache(shaderCache),
          mTextureCache(textureCache)
    {
        LOG_TRACE( "MaterialCache: Constructing" );
    }

    MaterialCache::~MaterialCache
    ()
    {
        LOG_TRACE( "MaterialCache: Destructing" );
    }

    SharedAssetRuntime*
    MaterialCache::loadRuntime
    (AssetDefinition* def)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (!def)
        {
            LOG_ERROR("MaterialCache: Material Definition is null");
            return nullptr;
        }

        MaterialDefinition* matDef = static_cast<MaterialDefinition*>(def);
        MaterialRuntime* material = new MaterialRuntime(matDef,mProjectRuntime);

        if(!material->useDefinition())
        {
            LOG_ERROR("MaterialCache: Material useDefinition Failed");
           	delete material;
            material = nullptr;
        }
        else
        {
        	mRuntimes.push_back(material);
        }

        return material;
    }
} // End of Dream

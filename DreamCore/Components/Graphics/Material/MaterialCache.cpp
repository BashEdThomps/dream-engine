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

#include "../../../Project/Project.h"
#include "../../../Project/ProjectDirectory.h"

#include "../Texture/TextureDefinition.h"
#include "../Texture/TextureRuntime.h"
#include "../Texture/TextureCache.h"
#include "../Shader/ShaderRuntime.h"
#include "../Shader/ShaderCache.h"
#include "../.././../Project/ProjectRuntime.h"

namespace Dream
{
    MaterialCache::MaterialCache
    (ProjectRuntime* parent, ShaderCache* shaderCache, TextureCache* textureCache)
        : Cache(parent),
          mShaderCache(shaderCache),
          mTextureCache(textureCache)
    {
#ifdef DREAM_LOG
        setLogClassName("MaterialCache");
        auto log = getLog();
        log->trace( "Constructing" );
#endif
    }

    MaterialCache::~MaterialCache
    ()
    {
#ifdef DREAM_LOG
        auto log = getLog();
        log->trace( "Destructing" );
#endif
    }

    SharedAssetRuntime*
    MaterialCache::loadRuntime
    (AssetDefinition* def)
    {
#ifdef DREAM_LOG
        auto log = getLog();
#endif
        if (!def)
        {

#ifdef DREAM_LOG
            log->error("Material Definition is null");
#endif
            return nullptr;
        }
        auto matDef = static_cast<MaterialDefinition*>(def);
        auto shader = static_cast<ShaderRuntime*>(mShaderCache->getRuntime(matDef->getShader()));

        if (shader == nullptr)
        {
#ifdef DREAM_LOG
            log->error("Cannot create material {} with null shader", matDef->getNameAndUuidString());
#endif
            return nullptr;
        }

        auto material = new MaterialRuntime(matDef,mProjectRuntime);
        auto diffuse = static_cast<TextureRuntime*>(mTextureCache->getRuntime(matDef->getDiffuseTexture()));
        auto specular = static_cast<TextureRuntime*>(mTextureCache->getRuntime(matDef->getSpecularTexture()));
        auto normal = static_cast<TextureRuntime*>(mTextureCache->getRuntime(matDef->getNormalTexture()));
        auto displacement = static_cast<TextureRuntime*>(mTextureCache->getRuntime(matDef->getDisplacementTexture()));
        material->useDefinition();

        material->setDiffuseTexture(diffuse);
        material->setSpecularTexture(specular);
        material->setNormalTexture(normal);
        material->setDisplacementTexture(displacement);
        material->setShader(shader);
        shader->addMaterial(material);
        mRuntimes.push_back(material);
        return material;
    }
} // End of Dream

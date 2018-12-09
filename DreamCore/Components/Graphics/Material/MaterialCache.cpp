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
#include "MaterialInstance.h"

#include "../../../Project/Project.h"
#include "../../../Project/ProjectDirectory.h"

#include "../Texture/TextureDefinition.h"
#include "../Texture/TextureInstance.h"
#include "../Texture/TextureCache.h"
#include "../Shader/ShaderInstance.h"
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
        auto log = getLog();
        log->trace( "Constructing" );
        setLogClassName("MaterialCache");
    }

    MaterialCache::~MaterialCache
    ()
    {
        auto log = getLog();
        log->trace( "Destructing" );
    }

    SharedAssetInstance*
    MaterialCache::loadInstance
    (AssetDefinition* def)
    {
        auto log = getLog();
        if (!def)
        {
            log->error("Material Definition is null");
            return nullptr;
        }
        auto matDef = dynamic_cast<MaterialDefinition*>(def);
        auto shader = dynamic_cast<ShaderInstance*>(mShaderCache->getInstance(matDef->getShader()));

        if (shader == nullptr)
        {
            log->error("Cannot create material {} with null shader", matDef->getNameAndUuidString());
            return nullptr;
        }

        auto material = new MaterialInstance(matDef,mProjectRuntime);
        auto diffuse = dynamic_cast<TextureInstance*>(mTextureCache->getInstance(matDef->getDiffuseTexture()));
        auto specular = dynamic_cast<TextureInstance*>(mTextureCache->getInstance(matDef->getSpecularTexture()));
        auto normal = dynamic_cast<TextureInstance*>(mTextureCache->getInstance(matDef->getNormalTexture()));
        auto displacement = dynamic_cast<TextureInstance*>(mTextureCache->getInstance(matDef->getDisplacementTexture()));
        material->load();

        material->setDiffuseTexture(diffuse);
        material->setSpecularTexture(specular);
        material->setNormalTexture(normal);
        material->setDisplacementTexture(displacement);
        material->setShader(shader);
        shader->addMaterial(material);
        mInstances.push_back(material);
        return material;
    }



} // End of Dream

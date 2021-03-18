/*
 * MaterialDefinition.cpp
 *
 * Created: 11 2017 by Ashley
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

#include "MaterialDefinition.h"

#include "Common/Logger.h"
#include "Common/Constants.h"
#include "Math/Vector.h"


namespace octronic::dream
{
    MaterialDefinition::MaterialDefinition
    (const shared_ptr<ProjectDefinition>& pd, const json &js)
        : AssetDefinition("MaterialDefinition",pd,js)
    {
        LOG_TRACE("MaterialDefinition: Constructing");
    }

    MaterialDefinition::~MaterialDefinition()
    {
        LOG_TRACE("MaterialDefinition: Destructing");
    }

    // Shader ==================================================================

    UuidType
    MaterialDefinition::getShader
    ()
    const
    {
        if(!mJson[Constants::ASSET_ATTR_MATERIAL_SHADER].is_number())
        {
            return Uuid::INVALID;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_SHADER];
    }

    void
    MaterialDefinition::setShader
    (UuidType val)
    {
        mJson[Constants::ASSET_ATTR_MATERIAL_SHADER] = val;
    }

    // Albedo ==================================================================

    UuidType
    MaterialDefinition::getAlbedoTexture
    ()
    const
    {
        if(!mJson[Constants::ASSET_ATTR_MATERIAL_ALBEDO].is_number())
        {
            return Uuid::INVALID;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_ALBEDO];
    }

    void
    MaterialDefinition::setAlbedoTexture
    (UuidType val)
    {
        mJson[Constants::ASSET_ATTR_MATERIAL_ALBEDO]  = val;
    }

    // Normal ==================================================================

    UuidType
    MaterialDefinition::getNormalTexture
    ()
    const
    {
        if(!mJson[Constants::ASSET_ATTR_MATERIAL_NORMAL].is_number())
        {
            return Uuid::INVALID;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_NORMAL];
    }

    void
    MaterialDefinition::setNormalTexture
    (UuidType val)
    {
        mJson[Constants::ASSET_ATTR_MATERIAL_NORMAL]  = val;
    }

    // Metallic ================================================================

    UuidType
    MaterialDefinition::getMetallicTexture
    ()
    const
    {
        if(!mJson[Constants::ASSET_ATTR_MATERIAL_METALLIC].is_number())
        {
            return Uuid::INVALID;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_METALLIC];
    }

    void
    MaterialDefinition::setMetallicTexture
    (UuidType val)
    {
        mJson[Constants::ASSET_ATTR_MATERIAL_METALLIC] = val;
    }

    // Roughness ===============================================================

    UuidType
    MaterialDefinition::getRoughnessTexture
    ()
    const
    {
        if(!mJson[Constants::ASSET_ATTR_MATERIAL_ROUGHNESS].is_number())
        {
            return Uuid::INVALID;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_ROUGHNESS];
    }

    void
    MaterialDefinition::setRoughnessTexture
    (UuidType val)
    {
        mJson[Constants::ASSET_ATTR_MATERIAL_ROUGHNESS] = val;
    }

    // Ao ======================================================================

    UuidType
    MaterialDefinition::getAoTexture
    ()
    const
    {
        if(!mJson[Constants::ASSET_ATTR_MATERIAL_AO].is_number())
        {
            return Uuid::INVALID;
        }
        return mJson[Constants::ASSET_ATTR_MATERIAL_AO];
    }

    void
    MaterialDefinition::setAoTexture
    (UuidType val)
    {
        mJson[Constants::ASSET_ATTR_MATERIAL_AO]  = val;
    }
}


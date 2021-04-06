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
  (ProjectDefinition& pd, const json &js)
    : AssetDefinition(pd,js)
  {
    LOG_TRACE("MaterialDefinition: Constructing");
  }

  // Shader ==================================================================

  UuidType
  MaterialDefinition::getShaderUuid
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
  MaterialDefinition::setShaderUuid
  (UuidType val)
  {
    mJson[Constants::ASSET_ATTR_MATERIAL_SHADER] = val;
  }

  // Albedo ==================================================================

  UuidType
  MaterialDefinition::getAlbedoTextureUuid
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
  MaterialDefinition::setAlbedoTextureUuid
  (UuidType val)
  {
    mJson[Constants::ASSET_ATTR_MATERIAL_ALBEDO]  = val;
  }

  // Normal ==================================================================

  UuidType
  MaterialDefinition::getNormalTextureUuid
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
  MaterialDefinition::setNormalTextureUuid
  (UuidType val)
  {
    mJson[Constants::ASSET_ATTR_MATERIAL_NORMAL]  = val;
  }

  // Metallic ================================================================

  UuidType
  MaterialDefinition::getMetallicTextureUuid
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
  MaterialDefinition::setMetallicTextureUuid
  (UuidType val)
  {
    mJson[Constants::ASSET_ATTR_MATERIAL_METALLIC] = val;
  }

  // Roughness ===============================================================

  UuidType
  MaterialDefinition::getRoughnessTextureUuid
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
  MaterialDefinition::setRoughnessTextureUuid
  (UuidType val)
  {
    mJson[Constants::ASSET_ATTR_MATERIAL_ROUGHNESS] = val;
  }

  // Ao ======================================================================

  UuidType
  MaterialDefinition::getAoTextureUuid
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
  MaterialDefinition::setAoTextureUuid
  (UuidType val)
  {
    mJson[Constants::ASSET_ATTR_MATERIAL_AO]  = val;
  }
}


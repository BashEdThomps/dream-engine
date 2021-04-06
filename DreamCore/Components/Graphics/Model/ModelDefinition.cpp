/*
 * ModelDefinition.cpp
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
#include "ModelDefinition.h"

#include "Common/Logger.h"
#include "Common/Constants.h"

using nlohmann::json;


namespace octronic::dream
{
  ModelDefinition::ModelDefinition
  (ProjectDefinition& pd,
   const json &js)
    : AssetDefinition(pd,js)
  {
    LOG_TRACE("ModelDefinition: Constructing {}", getNameAndUuidString());
  }

  bool
  ModelDefinition::isFormatAssimp
  ()
  {
    return getFormat() == Constants::ASSET_FORMAT_MODEL_ASSIMP;
  }

  bool // Indicates whether a new insertion was made
  ModelDefinition::addModelMaterial
  (const string &material, UuidType shader)
  {
    if (mJson.find(Constants::ASSET_ATTR_MODEL_MATERIAL_LIST) == mJson.end())
    {
      mJson[Constants::ASSET_ATTR_MODEL_MATERIAL_LIST] = json::array();
    }

    for (json& matShad : mJson[Constants::ASSET_ATTR_MODEL_MATERIAL_LIST])
    {
      if (matShad.is_object() && matShad[Constants::ASSET_ATTR_MODEL_MODEL_MATERIAL] == material)
      {
        matShad[Constants::ASSET_ATTR_MODEL_DREAM_MATERIAL] = shader;
        return false;
      }
    }

    auto shaderJson = json::object();
    shaderJson[Constants::ASSET_ATTR_MODEL_MODEL_MATERIAL] = material;
    shaderJson[Constants::ASSET_ATTR_MODEL_DREAM_MATERIAL] = shader;
    mJson[Constants::ASSET_ATTR_MODEL_MATERIAL_LIST].push_back(shaderJson);
    return true;
  }

  json
  ModelDefinition::getModelMaterials
  ()
  const
  {
    if(mJson.find(Constants::ASSET_ATTR_MODEL_MATERIAL_LIST) == mJson.end())
    {
      return json::array();
    }
    return mJson[Constants::ASSET_ATTR_MODEL_MATERIAL_LIST];
  }

  void
  ModelDefinition::removeModelMaterial
  (const string &material)
  {
    auto shaderMap = mJson[Constants::ASSET_ATTR_MODEL_MATERIAL_LIST];
    for (auto nextShader : shaderMap)
    {
      auto materialName = nextShader[Constants::ASSET_ATTR_MODEL_MODEL_MATERIAL];
      if (materialName.is_string())
      {
        string materialNameStr = materialName;
        if (material == materialNameStr)
        {
          LOG_DEBUG("ModelDefinition: Removing material form {} shader map {}",getName(),material);
          shaderMap.erase(find(begin(shaderMap),end(shaderMap),nextShader));
        }
      }
    }
    LOG_ERROR("ModelDefinition: Could not remove {} from {} shader map, object not found",getName(), material);
  }

  void
  ModelDefinition::clearModelMaterialList
  ()
  {
    mJson[Constants::ASSET_ATTR_MODEL_MATERIAL_LIST].clear();
  }

  UuidType
  ModelDefinition::getDreamMaterialForModelMaterial
  (const string &mat)
  {
    auto shaderMap = mJson[Constants::ASSET_ATTR_MODEL_MATERIAL_LIST];
    for (auto nextShader : shaderMap)
    {
      auto materialName = nextShader[Constants::ASSET_ATTR_MODEL_MODEL_MATERIAL];
      if (materialName.is_string())
      {
        string materialNameStr = materialName;
        if (mat == materialNameStr)
        {
          if (!nextShader[Constants::ASSET_ATTR_MODEL_DREAM_MATERIAL].is_number())
          {
            nextShader[Constants::ASSET_ATTR_MODEL_DREAM_MATERIAL] = Uuid::INVALID;
          }
          return nextShader[Constants::ASSET_ATTR_MODEL_DREAM_MATERIAL];
        }
      }
    }
    return Uuid::INVALID;
  }
}

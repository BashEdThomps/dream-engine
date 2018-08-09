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
#include <spdlog/spdlog.h>
#include <map>

using std::find;
using std::begin;
using std::end;
using std::pair;
using nlohmann::json;

namespace Dream
{
    ModelDefinition::ModelDefinition
    (ProjectDefinition* pdHandle, json js)
        : IAssetDefinition(pdHandle,js)
    {

        auto log = spdlog::get("ModelDefinition");
        if (log == nullptr)
        {
            log = spdlog::stdout_color_mt("ModelDefinition");
        }
        log->info("Constructing");
    }

    bool
    ModelDefinition::isFormatAssimp
    ()
    {
        return getFormat().compare(Constants::ASSET_FORMAT_MODEL_ASSIMP) == 0;
    }

    bool // Indicates whether a new insertion was made
    ModelDefinition::addMaterialShader
    (string material, string shader)
    {
        if (mJson[Constants::ASSET_ATTR_MODEL_MATERIAL_SHADER_LIST].is_null())
        {
             mJson[Constants::ASSET_ATTR_MODEL_MATERIAL_SHADER_LIST] = json::array();
        }

        for (json matShad : mJson[Constants::ASSET_ATTR_MODEL_MATERIAL_SHADER_LIST])
        {
            if (matShad.is_object() && matShad[Constants::ASSET_ATTR_MODEL_MATERIAL] == material)
            {
                matShad[Constants::ASSET_ATTR_MODEL_SHADER] = shader;
                return false;
            }
        }

        auto shaderJson = json::object();
        shaderJson[Constants::ASSET_ATTR_MODEL_MATERIAL] = material;
        shaderJson[Constants::ASSET_ATTR_MODEL_SHADER] = shader;
        mJson[Constants::ASSET_ATTR_MODEL_MATERIAL_SHADER_LIST].push_back(shaderJson);
        return true;
    }

    json*
    ModelDefinition::getMaterialShaders
    ()
    {
        if(mJson[Constants::ASSET_ATTR_MODEL_MATERIAL_SHADER_LIST].is_null())
        {
            mJson[Constants::ASSET_ATTR_MODEL_MATERIAL_SHADER_LIST] = json::array();
        }
        return &mJson[Constants::ASSET_ATTR_MODEL_MATERIAL_SHADER_LIST];
    }

    void
    ModelDefinition::removeMaterialShader
    (string material)
    {
        auto log = spdlog::get("ModelDefinition");
        auto shaderMap = mJson[Constants::ASSET_ATTR_MODEL_MATERIAL_SHADER_LIST];
        for (auto nextShader : shaderMap)
        {
            auto materialName = nextShader[Constants::ASSET_ATTR_MODEL_MATERIAL];
            if (materialName.is_string())
            {
                string materialNameStr = materialName;
                if (material.compare(materialNameStr) == 0)
                {
                    log->info("Removing material form {} shader map {}",getName(),material);
                    shaderMap.erase(find(begin(shaderMap),end(shaderMap),nextShader));
                }
            }
        }
        log->error("Could not remove {} from {} shader map, object not found",getName(), material);
    }

    void ModelDefinition::clearMaterialShaderList()
    {
        mJson[Constants::ASSET_ATTR_MODEL_MATERIAL_SHADER_LIST].clear();
    }
}

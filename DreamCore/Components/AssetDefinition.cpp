/*
* AssetDefinition
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string>
#include  <sstream>
#include "AssetDefinition.h"

using std::cout;
using std::endl;

namespace Dream
{
    AssetDefinition::AssetDefinition
    (ProjectDefinition* parent, json jsonDef)
        : Definition(jsonDef),
          mProjectDefinition(parent)
    {
        setLogClassName("AssetDefinition");
        auto log = getLog();
        log->trace("Constructing {}", getNameAndUuidString());
    }

    AssetDefinition::~AssetDefinition
    ()
    {
        auto log = getLog();
        log->trace("Destructing {}", getNameAndUuidString());
    }

    AssetType
    AssetDefinition::getAssetType
    ()
    {
        return Constants::getAssetTypeEnumFromString(mJson[Constants::ASSET_TYPE]);
    }

    void
    AssetDefinition::setType
    (string type)
    {
        mJson[Constants::ASSET_TYPE] = type;
    }

    string
    AssetDefinition::getType
    ()
    {
        if (mJson[Constants::ASSET_TYPE].is_null())
        {
            mJson[Constants::ASSET_TYPE] = "";
        }
        return mJson[Constants::ASSET_TYPE];
    }

    void
    AssetDefinition::setFormat
    (string format)
    {
        mJson[Constants::ASSET_FORMAT] = format;
    }

    string
    AssetDefinition::getFormat
    ()
    {
        if (mJson[Constants::ASSET_FORMAT].is_null())
        {
            mJson[Constants::ASSET_FORMAT] = "";
        }

        return mJson[Constants::ASSET_FORMAT];
    }

    bool
    AssetDefinition::isTypeLight
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_LIGHT) == 0;
    }

    bool AssetDefinition::isTypeMaterial()
    {
       return getType().compare(Constants::ASSET_TYPE_MATERIAL) == 0;
    }

    bool
    AssetDefinition::isTypeFont
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_FONT) == 0;
    }

    bool
    AssetDefinition::isTypePhysicsObject
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_PHYSICS_OBJECT) == 0;
    }

    bool
    AssetDefinition::isTypeTexture
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_TEXTURE) == 0;
    }

    bool
    AssetDefinition::isTypePath
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_PATH) == 0;
    }

    bool
    AssetDefinition::isTypeParticleEmitter
    ()
    {
       return getType().compare(Constants::ASSET_TYPE_PARTICLE_EMITTER) == 0;
    }

    bool
    AssetDefinition::isTypeAudio
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_AUDIO) == 0;
    }

    bool
    AssetDefinition::isTypeModel
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_MODEL) == 0;
    }

    bool
    AssetDefinition::isTypeScript
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_SCRIPT) == 0;
    }

    bool
    AssetDefinition::isTypeShader
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_SHADER) == 0;
    }

    string
    AssetDefinition::getAssetTypeDirectory
    ()
    {
        if (isTypeAudio())
        {
            return Constants::ASSET_TYPE_AUDIO;
        }

        else if (isTypeFont())
        {
            return Constants::ASSET_TYPE_FONT;
        }

        else if (isTypeLight())
        {
            return Constants::ASSET_TYPE_LIGHT;
        }

        else if (isTypeMaterial())
        {
            return Constants::ASSET_TYPE_MATERIAL;
        }

        else if (isTypeModel())
        {
            return Constants::ASSET_TYPE_MODEL;
        }

        else if (isTypeParticleEmitter())
        {
            return Constants::ASSET_TYPE_PARTICLE_EMITTER;
        }

        else if (isTypePath())
        {
            return Constants::ASSET_TYPE_PATH;
        }

        else if (isTypePhysicsObject())
        {
            return Constants::ASSET_TYPE_PHYSICS_OBJECT;
        }

        else if (isTypeScript())
        {
            return Constants::ASSET_TYPE_SCRIPT;
        }

        else if (isTypeShader())
        {
            return Constants::ASSET_TYPE_SHADER;
        }

        else if (isTypeTexture())
        {
            return Constants::ASSET_TYPE_TEXTURE;
        }

        else
        {
            return "Type Not Found";
        }
    }

    string
    AssetDefinition::getAssetPath
    ()
    {
        stringstream pathStream;
        pathStream << Constants::DIR_PATH_SEP
                   << getAssetTypeDirectory() << Constants::DIR_PATH_SEP
                   << getUuid() << Constants::DIR_PATH_SEP
                   << getFormat();
        return pathStream.str();
    }

    string
    AssetDefinition::getGroup
    ()
    {
        if (mJson[Constants::ASSET_ATTR_GROUP].is_null())
        {
            mJson[Constants::ASSET_ATTR_GROUP] = Constants::ASSET_ATTR_GROUP_DEFAULT;
        }
        return mJson[Constants::ASSET_ATTR_GROUP];
    }

    void
    AssetDefinition::setGroup
    (string group)
    {
        mJson[Constants::ASSET_ATTR_GROUP] = group;
    }


    void
    AssetDefinition::showStatus
    ()
    {
        auto log = getLog();
        log->debug( mJson.dump(1) );
    }

    ProjectDefinition*
    AssetDefinition::getProject
    ()
    {
        return mProjectDefinition;
    }

} // End of Dream

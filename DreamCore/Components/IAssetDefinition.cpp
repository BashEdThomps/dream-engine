/*
* IAssetDefinition
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
#include "IAssetDefinition.h"
#include "../Common/Constants.h"

using std::cout;
using std::endl;

namespace Dream
{
    IAssetDefinition::IAssetDefinition
    (ProjectDefinition* parent, json jsonDef)
        : IDefinition(jsonDef),
          mProjectDefinition(parent)
    {
        setLogClassName("AssetDefinition");
        auto log = getLog();
        log->trace("Constructing {}", getNameAndUuidString());
    }

    IAssetDefinition::~IAssetDefinition
    ()
    {
        auto log = getLog();
        log->trace("Destructing {}", getNameAndUuidString());
    }

    AssetType
    IAssetDefinition::getAssetType
    (json js)
    {
        return Constants::getAssetTypeEnumFromString(js[Constants::ASSET_TYPE]);
    }

    void
    IAssetDefinition::setType
    (string type)
    {
        mJson[Constants::ASSET_TYPE] = type;
    }

    string
    IAssetDefinition::getType
    ()
    {
        if (mJson[Constants::ASSET_TYPE].is_null())
        {
            mJson[Constants::ASSET_TYPE] = "";
        }
        return mJson[Constants::ASSET_TYPE];
    }

    void
    IAssetDefinition::setFormat
    (string format)
    {
        mJson[Constants::ASSET_FORMAT] = format;
    }

    string
    IAssetDefinition::getFormat
    ()
    {
        if (mJson[Constants::ASSET_FORMAT].is_null())
        {
            mJson[Constants::ASSET_FORMAT] = "";
        }

        return mJson[Constants::ASSET_FORMAT];
    }

    bool
    IAssetDefinition::isTypeLight
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_LIGHT) == 0;
    }

    bool IAssetDefinition::isTypeMaterial()
    {
       return getType().compare(Constants::ASSET_TYPE_MATERIAL) == 0;
    }

    bool
    IAssetDefinition::isTypeFont
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_FONT) == 0;
    }

    bool
    IAssetDefinition::isTypePhysicsObject
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_PHYSICS_OBJECT) == 0;
    }

    bool
    IAssetDefinition::isTypeTexture
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_TEXTURE) == 0;
    }

    bool
    IAssetDefinition::isTypePath
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_PATH) == 0;
    }

    bool
    IAssetDefinition::isTypeParticleEmitter
    ()
    {
       return getType().compare(Constants::ASSET_TYPE_PARTICLE_EMITTER) == 0;
    }

    bool
    IAssetDefinition::isTypeAudio
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_AUDIO) == 0;
    }

    bool
    IAssetDefinition::isTypeModel
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_MODEL) == 0;
    }

    bool
    IAssetDefinition::isTypeScript
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_SCRIPT) == 0;
    }

    bool
    IAssetDefinition::isTypeShader
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_SHADER) == 0;
    }

    string
    IAssetDefinition::getAssetTypeDirectory
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
    IAssetDefinition::getAssetPath
    ()
    {
        stringstream pathStream;
        pathStream << Constants::DIR_PATH_SEP
                   << Constants::ASSET_DIR << Constants::DIR_PATH_SEP
                   << getAssetTypeDirectory() << Constants::DIR_PATH_SEP
                   << getUuid() << Constants::DIR_PATH_SEP
                   << getFormat();
        return pathStream.str();
    }


    void
    IAssetDefinition::showStatus
    ()
    {
        auto log = getLog();
        log->debug( mJson.dump(1) );
    }

    ProjectDefinition*
    IAssetDefinition::getProject
    ()
    {
        return mProjectDefinition;
    }

} // End of Dream

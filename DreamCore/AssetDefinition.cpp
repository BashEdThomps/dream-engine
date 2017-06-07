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

#include <strings.h>
#include "AssetDefinition.h"

namespace Dream
{
    AssetDefinition::AssetDefinition(nlohmann::json jsonDef)
    {
        mJson = jsonDef;
        loadMetadata();
        showStatus();
    }

    AssetDefinition::~AssetDefinition(void)
    {
        if (Constants::DEBUG)
        {
            cout << "AssetDefinition: Destroying Object" << endl;
        }
    }

    void AssetDefinition::setProjectPath(string path)
    {
        mProjectPath = path;
    }

    string AssetDefinition::getProjectPath()
    {
        return mProjectPath;
    }

    void AssetDefinition::loadMetadata()
    {
        // UUID
        if (mJson[Constants::ASSET_UUID].is_null())
        {

            if (Constants::DEBUG)
            {
                cerr << "AssetDefinition: Construction Asset from JSON with null UUID." << endl;
            }
        }
        else
        {
            setUuid(mJson[Constants::ASSET_UUID]);
        }
        // Name
        if (mJson[Constants::ASSET_NAME].is_null())
        {
            if (Constants::DEBUG)
            {
                cerr << "AssetDefinition: Construction Asset from JSON with null Name." << endl;
            }
        }
        else
        {
            setName(mJson[Constants::ASSET_NAME]);
        }
        // Type
        if (mJson[Constants::ASSET_TYPE].is_null())
        {
            if (Constants::DEBUG)
            {
                cerr << "AssetDefinition: Construction Asset from JSON with null Type" << endl;
            }
        }
        else
        {
            setType(mJson[Constants::ASSET_TYPE]);
        }
        // Format
        if (mJson[Constants::ASSET_FORMAT].is_null())
        {
            if (Constants::DEBUG)
            {
                cerr << "AssetDefinition: Construction Asset from JSON with null Format" << endl;
            }
        }
        else
        {
            setFormat(mJson[Constants::ASSET_FORMAT]);
        }
    }

    pair<string,string> AssetDefinition::mapPair(string key, string value)
    {
        return pair<string,string>(key,value);
    }

    void AssetDefinition::setUuid(string uuid)
    {
        addAttribute(Constants::ASSET_UUID, uuid);
    }

    bool AssetDefinition::hasUuid(string uuid)
    {
        return getUuid().compare(uuid) == 0;
    }

    void AssetDefinition::addAttribute(string key, string value)
    {
        mAttributes.insert(mapPair(key,value));
    }

    string AssetDefinition::getAttribute(string attribute)
    {
        try
        {
            return mAttributes.at(attribute);
        }
        catch (const exception &ex)
        {
            if (Constants::DEBUG)
            {
                cerr << "AssetDefinition: No Attribute - " << attribute << endl;
            }
            return "";
        }
    }

    int AssetDefinition::getAttributeAsInt(string attr)
    {
        return stoi(getAttribute(attr));
    }

    float AssetDefinition::getAttributeAsFloat(string attr)
    {
        return stof(getAttribute(attr));
    }

    double AssetDefinition::getAttributeAsDouble(string attr)
    {
        return stod(getAttribute(attr));
    }

    bool AssetDefinition::getAttributeAsBool(string attr)
    {
        return strcasecmp("true",getAttribute(attr).c_str()) == 0;
    }

    long AssetDefinition::getAttributeAsLong(string attr)
    {
        return stol(getAttribute(attr));
    }

    char AssetDefinition::getAttributeAsChar(string attr)
    {
        return getAttribute(attr).c_str()[0];
    }

    string AssetDefinition::getUuid()
    {
        return getAttribute(Constants::ASSET_UUID);
    }

    void AssetDefinition::setName(string name)
    {
        addAttribute(Constants::ASSET_NAME,name);
    }

    string AssetDefinition::getName()
    {
        return getAttribute(Constants::ASSET_NAME);
    }

    void AssetDefinition::setType(string type)
    {
        addAttribute(Constants::ASSET_TYPE,type);
    }

    string AssetDefinition::getType()
    {
        return getAttribute(Constants::ASSET_TYPE);
    }

    void AssetDefinition::setFormat(string format)
    {
        addAttribute(Constants::ASSET_FORMAT,format);
    }

    string AssetDefinition::getFormat()
    {
        return getAttribute(Constants::ASSET_FORMAT);
    }

    bool AssetDefinition::isTypeLight()
    {
        return getType().compare(Constants::ASSET_TYPE_LIGHT) == 0;
    }

    bool AssetDefinition::isTypeFont()
    {
        return getType().compare(Constants::ASSET_TYPE_FONT) == 0;
    }

    bool AssetDefinition::isTypePhysicsObject()
    {
        return getType().compare(Constants::ASSET_TYPE_PHYSICS_OBJECT) == 0;
    }

    bool AssetDefinition::isTypeAnimation()
    {
        return getType().compare(Constants::ASSET_TYPE_ANIMATION) == 0;
    }

    bool AssetDefinition::isTypeAudio()
    {
        return getType().compare(Constants::ASSET_TYPE_AUDIO) == 0;
    }

    bool AssetDefinition::isTypeModel()
    {
        return getType().compare(Constants::ASSET_TYPE_MODEL) == 0;
    }

    bool AssetDefinition::isTypeScript()
    {
        return getType().compare(Constants::ASSET_TYPE_SCRIPT) == 0;
    }

    bool AssetDefinition::isTypeShader()
    {
        return getType().compare(Constants::ASSET_TYPE_SHADER) == 0;
    }

    bool AssetDefinition::isTypeSprite()
    {
        return getType().compare(Constants::ASSET_TYPE_SPRITE) == 0;
    }

    bool AssetDefinition::isScriptFormatLua()
    {
        return getFormat().compare(Constants::ASSET_FORMAT_SCRIPT_LUA) == 0;
    }

    bool AssetDefinition::isModelFormatAssimp()
    {
        return getFormat().compare(Constants::ASSET_FORMAT_MODEL_ASSIMP) == 0;
    }

    bool AssetDefinition::isAudioFormatMusic()
    {
        return getFormat().compare(Constants::ASSET_FORMAT_AUDIO_MUSIC) == 0;
    }

    bool AssetDefinition::isAudioFormatSoundEffect()
    {
        return getFormat().compare(Constants::ASSET_FORMAT_AUDIO_SFX) == 0;
    }

    string AssetDefinition::getAssetTypeDirectory()
    {
        string type = getType();
        if (isTypeAnimation())
        {
            return Constants::ASSET_TYPE_ANIMATION;
        }
        else if (isTypeAudio())
        {
            return Constants::ASSET_TYPE_AUDIO;
        }
        else if (isTypeModel())
        {
            return Constants::ASSET_TYPE_MODEL;
        }
        else if (isTypeScript())
        {
            return Constants::ASSET_TYPE_SCRIPT;
        }
        else if (isTypeShader())
        {
            return Constants::ASSET_TYPE_SHADER;
        }
        else if (isTypeSprite())
        {
            return Constants::ASSET_TYPE_SPRITE;
        }
        else if (isTypeFont())
        {
            return Constants::ASSET_TYPE_FONT;
        }
        else if (isTypePhysicsObject())
        {
            return Constants::ASSET_TYPE_PHYSICS_OBJECT;
        }
        else
        {
            return "Type Not Found";
        }
    }

    string AssetDefinition::getNameAndUuidString()
    {
        return getName() + " (" + getUuid() + ")";
    }

    string AssetDefinition::getAssetPath()
    {
        stringstream pathStream;
        pathStream << Constants::DIR_PATH_SEP
                   << Constants::ASSET_DIR << Constants::DIR_PATH_SEP
                   << getAssetTypeDirectory() << Constants::DIR_PATH_SEP
                   << getUuid() << Constants::DIR_PATH_SEP
                   << getFormat();
        return pathStream.str();
    }

    void AssetDefinition::showStatus()
    {
        if (Constants::DEBUG)
        {
            cout << "AssetDefinition:" << endl;
        }
        for (const auto& any : mAttributes)
        {
            string value = any.second;
            if (Constants::DEBUG)
            {
                cout << "\t" << any.first << " : " << value << endl;
            }
        }
    }

    nlohmann::json AssetDefinition::toJson()
    {
        return mJson;
    }

    AssetDefinition*
    AssetDefinition::getAssetDefinitionByUuid
    (vector<AssetDefinition*> definitions, string uuid)
    {
        for (AssetDefinition* ad : definitions)
        {
            if (ad->hasUuid(uuid))
            {
                return ad;
            }
        }
        return nullptr;
    }
} // End of Dream

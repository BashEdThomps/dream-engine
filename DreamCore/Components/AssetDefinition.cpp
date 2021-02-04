/*
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

#include "AssetDefinition.h"

#include "Common/Constants.h"
#include "Common/Logger.h"
#include "Project/ProjectDefinition.h"

#include <string>
#include <sstream>
#include <regex>

using std::regex;
using std::cmatch;

namespace octronic::dream
{
    AssetDefinition::AssetDefinition
    (const string& className, ProjectDefinition* parent, const json &jsonDef)
        : Definition(className, jsonDef),
          mProjectDefinition(parent)
    {
        LOG_TRACE("AssetDefinition: Constructing {}", getNameAndUuidString());
    }

    AssetDefinition::~AssetDefinition
    ()
    {
        LOG_TRACE("AssetDefinition: Destructing {}", getNameAndUuidString());
    }

    AssetType
    AssetDefinition::getAssetType
    ()
    {
        if(dreamTryLock()){
            dreamLock();
            return Constants::getAssetTypeEnumFromString(mJson[Constants::ASSET_TYPE]);
        } dreamElseLockFailed
    }

    void
    AssetDefinition::setType
    (string type)
    {
        if(dreamTryLock()){
            dreamLock();
            mJson[Constants::ASSET_TYPE] = type;
        } dreamElseLockFailed
    }

    string
    AssetDefinition::getType
    ()
    {
        if(dreamTryLock()){
            dreamLock();
            if (mJson.find(Constants::ASSET_TYPE) == mJson.end())
            {
                mJson[Constants::ASSET_TYPE] = "";
            }
            return mJson[Constants::ASSET_TYPE];
        } dreamElseLockFailed
    }

    void
    AssetDefinition::setFormat
    (string format)
    {
        if(dreamTryLock()){
            dreamLock();
            mJson[Constants::ASSET_FORMAT] = format;
        } dreamElseLockFailed
    }

    string
    AssetDefinition::getFormat
    ()
    {
        if(dreamTryLock()){
            dreamLock();
            if (mJson.find(Constants::ASSET_FORMAT) == mJson.end())
            {
                mJson[Constants::ASSET_FORMAT] = "";
            }

            return mJson[Constants::ASSET_FORMAT];
        } dreamElseLockFailed
    }

    bool
    AssetDefinition::isTypeAnimation
    ()
    {
        if(dreamTryLock()){
            dreamLock();
            return getType() == Constants::ASSET_TYPE_ANIMATION;
        } dreamElseLockFailed
    }

    bool
    AssetDefinition::isTypeLight
    ()
    {
        if(dreamTryLock()){
            dreamLock();
            return getType() == Constants::ASSET_TYPE_LIGHT;
        } dreamElseLockFailed
    }

    bool AssetDefinition::isTypeMaterial()
    {
        if(dreamTryLock()){
            dreamLock();
            return getType() == Constants::ASSET_TYPE_MATERIAL;
        } dreamElseLockFailed
    }

    bool
    AssetDefinition::isTypeFont
    ()
    {
        if(dreamTryLock()){
            dreamLock();
            return getType() == Constants::ASSET_TYPE_FONT;
        } dreamElseLockFailed
    }

    bool
    AssetDefinition::isTypePhysicsObject
    ()
    {
        if(dreamTryLock()){
            dreamLock();
            return getType() == Constants::ASSET_TYPE_PHYSICS_OBJECT;
        } dreamElseLockFailed
    }

    bool
    AssetDefinition::isTypeTexture
    ()
    {
        if(dreamTryLock()){
            dreamLock();
            return getType() == Constants::ASSET_TYPE_TEXTURE;
        } dreamElseLockFailed
    }

    bool
    AssetDefinition::isTypePath
    ()
    {
        if(dreamTryLock()){
            dreamLock();
            return getType() == Constants::ASSET_TYPE_PATH;
        } dreamElseLockFailed
    }

    bool
    AssetDefinition::isTypeAudio
    ()
    {
        if(dreamTryLock()){
            dreamLock();
            return getType() == Constants::ASSET_TYPE_AUDIO;
        } dreamElseLockFailed
    }

    bool
    AssetDefinition::isTypeModel
    ()
    {
        if(dreamTryLock()){
            dreamLock();
            return getType() == Constants::ASSET_TYPE_MODEL;
        } dreamElseLockFailed
    }

    bool
    AssetDefinition::isTypeScript
    ()
    {
        if(dreamTryLock()){
            dreamLock();
            return getType() == Constants::ASSET_TYPE_SCRIPT;
        } dreamElseLockFailed
    }

    bool
    AssetDefinition::isTypeShader
    ()
    {
        if(dreamTryLock()){
            dreamLock();
            return getType() == Constants::ASSET_TYPE_SHADER;
        } dreamElseLockFailed
    }

    string
    AssetDefinition::getGroup
    ()
    {
        if(dreamTryLock()){
            dreamLock();
            if (mJson.find(Constants::ASSET_ATTR_GROUP) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_GROUP] = Constants::ASSET_ATTR_GROUP_DEFAULT;
            }
            return mJson[Constants::ASSET_ATTR_GROUP];
        } dreamElseLockFailed
    }

    void
    AssetDefinition::setGroup
    (string group)
    {
        if(dreamTryLock()){
            dreamLock();
            mJson[Constants::ASSET_ATTR_GROUP] = group;
        } dreamElseLockFailed
    }


    ProjectDefinition*
    AssetDefinition::getProject
    ()
    {
        if(dreamTryLock()){
            dreamLock();
            return mProjectDefinition;
        } dreamElseLockFailed
    }

    AssetDefinition*
    AssetDefinition::duplicate
    ()
    {
        if(dreamTryLock()){
            dreamLock();
            auto newAD = mProjectDefinition->createNewAssetDefinition(getAssetType());
            newAD->mJson = mJson;
            newAD->setUuid(Uuid::generateUuid());
            string name = newAD->getName();
            regex numRegex("(\\d+)$");
            cmatch match;
            string resultStr;
            auto num = -1;

            if (regex_search(name.c_str(),match,numRegex))
            {
                resultStr = match[0].str();
                num = atoi(resultStr.c_str());
            }

            if (num > -1)
            {
                num++;
                name = name.substr(0,name.length()-resultStr.length());
                name.append(std::to_string(num));
                newAD->setName(name);
            }
            else
            {
                newAD->setName(getName()+".1");
            }

            return newAD;
        } dreamElseLockFailed
    }
}

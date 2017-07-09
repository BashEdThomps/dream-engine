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

#include "AssetDefinition.h"

#include <strings.h>

#include "../Common/Constants.h"

using std::cout;
using std::endl;

namespace Dream
{
    AssetType
    AssetDefinition::getAssetType
    (json js)
    {
       return Constants::assetTypeFromString(js[Constants::ASSET_TYPE]);
    }

    AssetDefinition::AssetDefinition
    (ProjectDefinition* parent, json jsonDef)
        : IDefinition(jsonDef),
          mProjectDefinitionHandle(parent)
    {
        if (Constants::DEBUG)
        {
            cout << "AssetDefinition: Constructing "
                 << getNameAndUuidString() << endl;
        }
    }

   AssetDefinition::~AssetDefinition
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "AssetDefinition: Destructing "
                 << getNameAndUuidString() << endl;
        }
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

    void AssetDefinition::setFormat(string format)
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
    AssetDefinition::isTypeAnimation
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_ANIMATION) == 0;
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

    bool
    AssetDefinition::isTypeSprite
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_SPRITE) == 0;
    }

    bool
    AssetDefinition::isScriptFormatLua
    ()
    {
        return getFormat().compare(Constants::ASSET_FORMAT_SCRIPT_LUA) == 0;
    }

    bool
    AssetDefinition::isModelFormatAssimp
    ()
    {
        return getFormat().compare(Constants::ASSET_FORMAT_MODEL_ASSIMP) == 0;
    }

    bool
    AssetDefinition::isAudioFormatWav
    ()
    {
        return getFormat().compare(Constants::ASSET_FORMAT_AUDIO_WAV) == 0;
    }

    bool
    AssetDefinition::isAudioFormatOgg
    ()
    {
        return getFormat().compare(Constants::ASSET_FORMAT_AUDIO_OGG) == 0;
    }

    void
    AssetDefinition::setAudioLoop
    (bool loop)
    {
        mJson[Constants::ASSET_ATTR_LOOP] = loop;
    }

    bool
    AssetDefinition::getAudioLoop
    ()
    {
        if (mJson[Constants::ASSET_ATTR_LOOP].is_null())
        {
            mJson[Constants::ASSET_ATTR_LOOP] = false;
        }
        return mJson[Constants::ASSET_ATTR_LOOP];
    }

    string
    AssetDefinition::getAssetTypeDirectory
    ()
    {
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

    string
    AssetDefinition::getAssetPath
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
    AssetDefinition::setColourRed
    (float value)
    {
       makeLightColourObject();
       mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::RED] = value;
    }

    float
    AssetDefinition::getColourRed
    ()
    {
        makeLightColourObject();
        if (mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::RED].is_null())
        {
            mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::RED] = 0;
        }

        return mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::RED];
    }

    void
    AssetDefinition::setColourGreen
    (float value)
    {
        makeLightColourObject();
        mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::GREEN] = value;
    }

    float
    AssetDefinition::getColourGreen
    ()
    {
        makeLightColourObject();
        if (mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::GREEN].is_null())
        {
            mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::GREEN] = 0;
        }

        return mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::GREEN];

    }

    void
    AssetDefinition::setColourBlue
    (float value)
    {
        makeLightColourObject();
       mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::BLUE] = value;
    }

    float
    AssetDefinition::getColourBlue
    ()
    {
        makeLightColourObject();
        if (mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::BLUE].is_null())
        {
            mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::BLUE] = 0;
        }

        return mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::BLUE];

    }

    void
    AssetDefinition::setColourAlpha
    (float value)
    {
        makeLightColourObject();
       mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::ALPHA] = value;
    }

    float
    AssetDefinition::getColourAlpha
    ()
    {
        makeLightColourObject();
        return mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::ALPHA];
    }

    void
    AssetDefinition::makeLightColourObject
    ()
    {
        if (mJson[Constants::ASSET_ATTR_LIGHT_COLOR].is_null())
        {
            mJson[Constants::ASSET_ATTR_LIGHT_COLOR] = json::object();
        }
    }

    void
    AssetDefinition::setSize
    (float size)
    {
       mJson[Constants::ASSET_ATTR_SIZE] = size;
    }

    float AssetDefinition::getSize()
    {
       if (mJson[Constants::ASSET_ATTR_SIZE].is_null())
       {
           mJson[Constants::ASSET_ATTR_SIZE] = 0;
       }

       return mJson[Constants::ASSET_ATTR_SIZE];
    }

    void
    AssetDefinition::setPhysicsObjectMass
    (float mass)
    {
        mJson[Constants::ASSET_ATTR_MASS] = mass;
    }

    float
    AssetDefinition::getPhysicsObjectMass
    ()
    {
        if (mJson[Constants::ASSET_ATTR_MASS].is_null())
        {
            mJson[Constants::ASSET_ATTR_MASS] = 0;
        }
        return mJson[Constants::ASSET_ATTR_MASS];
    }

    void
    AssetDefinition::setPhysicsObjectMargin
    (float margin)
    {
        mJson[Constants::ASSET_ATTR_MARGIN] = margin;
    }

    float
    AssetDefinition::getPhysicsObjectMargin
    ()
    {
        if (mJson[Constants::ASSET_ATTR_MARGIN].is_null())
        {
            mJson[Constants::ASSET_ATTR_MARGIN] = 0;
        }
        return mJson[Constants::ASSET_ATTR_MARGIN];
    }

    void
    AssetDefinition::setPhysicsObjectKinematic
    (bool kinematic)
    {
        mJson[Constants::ASSET_ATTR_KINEMATIC] = kinematic;
    }

    bool
    AssetDefinition::getPhysicsObjectKinematic
    ()
    {
        if (mJson[Constants::ASSET_ATTR_KINEMATIC].is_null())
        {
            mJson[Constants::ASSET_ATTR_KINEMATIC] = false;
        }
        return mJson[Constants::ASSET_ATTR_KINEMATIC];
    }

    void
    AssetDefinition::makeHalfExtentsObject
    ()
    {
        if (mJson[Constants::ASSET_ATTR_SIZE].is_null())
        {
            mJson[Constants::ASSET_ATTR_SIZE] = json::object();
        }
    }

    void
    AssetDefinition::setPhysicsObjectHalfExtentsX
    (float halfExtentX)
    {
        makeHalfExtentsObject();
        mJson[Constants::ASSET_ATTR_SIZE][Constants::X] = halfExtentX;
    }

    float
    AssetDefinition::getPhysicsObjectHalfExtentsX
    ()
    {
        makeHalfExtentsObject();
        if (mJson[Constants::ASSET_ATTR_SIZE][Constants::X].is_null())
        {
            mJson[Constants::ASSET_ATTR_SIZE][Constants::X] = 0;
        }
        return mJson[Constants::ASSET_ATTR_SIZE][Constants::X];
    }

    void
    AssetDefinition::setPhysicsObjectHalfExtentsY
    (float halfExtentY)
    {
        makeHalfExtentsObject();
        mJson[Constants::ASSET_ATTR_SIZE][Constants::Y] = halfExtentY;

    }

    float
    AssetDefinition::getPhysicsObjectHalfExtentsY
    ()
    {
        makeHalfExtentsObject();
        if (mJson[Constants::ASSET_ATTR_SIZE][Constants::Y].is_null())
        {
            mJson[Constants::ASSET_ATTR_SIZE][Constants::Y] = 0;
        }
        return mJson[Constants::ASSET_ATTR_SIZE][Constants::Y];
    }

    void
    AssetDefinition::setPhysicsObjectHalfExtentsZ
    (float halfExtentZ)
    {
        makeHalfExtentsObject();
        mJson[Constants::ASSET_ATTR_SIZE][Constants::Z] = halfExtentZ;
    }

    float
    AssetDefinition::getPhysicsObjectHalfExtentsZ
    ()
    {
        makeHalfExtentsObject();
        if (mJson[Constants::ASSET_ATTR_SIZE][Constants::Z].is_null())
        {
            mJson[Constants::ASSET_ATTR_SIZE][Constants::Z] = 0;
        }
        return mJson[Constants::ASSET_ATTR_SIZE][Constants::Z];
    }

    void
    AssetDefinition::makeNormalObject
    ()
    {
        if (mJson[Constants::ASSET_ATTR_NORMAL].is_null())
        {
            mJson[Constants::ASSET_ATTR_NORMAL] = json::object();
        }
    }

    void
    AssetDefinition::setPhysicsObjectNormalX
    (float halfExtentX)
    {
        makeNormalObject();
        mJson[Constants::ASSET_ATTR_NORMAL][Constants::X] = halfExtentX;
    }

    float
    AssetDefinition::getPhysicsObjectNormalX
    ()
    {
        makeNormalObject();
        if (mJson[Constants::ASSET_ATTR_NORMAL][Constants::X].is_null())
        {
            mJson[Constants::ASSET_ATTR_NORMAL][Constants::X] = 0;
        }
        return mJson[Constants::ASSET_ATTR_NORMAL][Constants::X];
    }

    void
    AssetDefinition::setPhysicsObjectNormalY
    (float halfExtentY)
    {
        makeNormalObject();
        mJson[Constants::ASSET_ATTR_NORMAL][Constants::Y] = halfExtentY;

    }

    float
    AssetDefinition::getPhysicsObjectNormalY
    ()
    {
        makeNormalObject();
        if (mJson[Constants::ASSET_ATTR_NORMAL][Constants::Y].is_null())
        {
            mJson[Constants::ASSET_ATTR_NORMAL][Constants::Y] = 0;
        }
        return mJson[Constants::ASSET_ATTR_NORMAL][Constants::Y];
    }

    void
    AssetDefinition::setPhysicsObjectNormalZ
    (float halfExtentZ)
    {
        makeNormalObject();
        mJson[Constants::ASSET_ATTR_NORMAL][Constants::Z] = halfExtentZ;
    }

    float
    AssetDefinition::getPhysicsObjectNormalZ
    ()
    {
        makeNormalObject();
        if (mJson[Constants::ASSET_ATTR_NORMAL][Constants::Z].is_null())
        {
            mJson[Constants::ASSET_ATTR_NORMAL][Constants::Z] = 0;
        }
        return mJson[Constants::ASSET_ATTR_NORMAL][Constants::Z];
    }

    void
    AssetDefinition::showStatus
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "AssetDefinition: " << mJson.dump(1) << endl;
        }
    }

    float
    AssetDefinition::getPhysicsObjectRadius
    ()
    {
        if (mJson[Constants::ASSET_ATTR_RADIUS].is_null())
        {
            mJson[Constants::ASSET_ATTR_RADIUS] = 0;
        }
        return mJson[Constants::ASSET_ATTR_RADIUS];
    }

    void
    AssetDefinition::setPhysicsObjectRadius
    (float rad)
    {
        mJson[Constants::ASSET_ATTR_RADIUS] = rad;
    }

    float
    AssetDefinition::getPhysicsObjectConstant
    ()
    {
        if (mJson[Constants::ASSET_ATTR_CONSTANT].is_null())
        {
            mJson[Constants::ASSET_ATTR_CONSTANT] = 0;
        }
        return mJson[Constants::ASSET_ATTR_CONSTANT];
    }

    void
    AssetDefinition::setPhysicsObjectConstant
    (float constant)
    {
        mJson[Constants::ASSET_ATTR_CONSTANT] = constant;
    }

    void
    AssetDefinition::addPhysicsObjectCompoundChild
    (CompoundChildDefinition def)
    {
       makeCompoundChildren();
       mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN].push_back(def.getJson());
    }

    vector<CompoundChildDefinition>
    AssetDefinition::getPhysicsObjectCompoundChildren
    ()
    {
        makeCompoundChildren();
        vector<CompoundChildDefinition> retval;
        for (json childJson : mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN])
        {
           CompoundChildDefinition def;
           def.uuid = childJson[Constants::UUID];
           def.transform = Transform3D(childJson[Constants::TRANSFORM]);
           retval.push_back(def);

        }
        return retval;
    }

    void
    AssetDefinition::removePhysicsObjectCompoundChild
    (CompoundChildDefinition def)
    {
        makeCompoundChildren();
        for
        (
            auto iter = begin(mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN]);
            iter != end(mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN]);
            iter++
       )
       {
            if (*iter == def.getJson())
            {
               mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN].erase(iter);
               return;
            }
        }
    }

    void
    AssetDefinition::makeCompoundChildren
    ()
    {
        if (mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN].is_null())
        {
            mJson[Constants::ASSET_ATTR_COMPOUND_CHILDREN] = json::array();
        }
    }

    ProjectDefinition*
    AssetDefinition::getProjectHandle
    ()
    {
        return mProjectDefinitionHandle;
    }

} // End of Dream

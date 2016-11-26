/*
* Dream::AssetDefinition
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

#ifndef ASSETDEFINITION_H
#define ASSETDEFINITION_H


#include <iostream>
#include <map>
#include <string>
#include <json.hpp>

#define ASSET_UUID "uuid"
#define ASSET_NAME "name"
#define ASSET_TYPE "type"
#define ASSET_FORMAT "format"

#define DIR_PATH_SEP "/"
#define ASSET_DIR "assets"

#define ASSET_DIR_ANIMATION "animation"
#define ASSET_DIR_AUDIO "audio"
#define ASSET_DIR_MODEL "model"
#define ASSET_DIR_SCRIPT "script"
#define ASSET_DIR_SHADER "shader"
#define ASSET_DIR_SPRITE "sprite"

#define ASSET_TYPE_ANIMATION "animation"
#define ASSET_TYPE_AUDIO "audio"
#define ASSET_TYPE_LIGHT "light"
#define ASSET_TYPE_MODEL "model"
#define ASSET_TYPE_PHYSICS_OBJECT "physicsObject"
#define ASSET_TYPE_SCRIPT "script"
#define ASSET_TYPE_SHADER "shader"
#define ASSET_TYPE_SPRITE "sprite"

#define ASSET_DEF_FORMAT_SCRIPT_CHAI "chai"
#define ASSET_DEF_FORMAT_MODEL_ASSIMP "assimp"
#define ASSET_DEF_FORMAT_AUDIO_WAV "wav"
#define ASSET_DEF_FORMAT_AUDIO_OGG "ogg"
#define ASSET_DEF_FORMAT_ANIMATION_DREAM "dream"

#define ASSET_ATTR_X "x"
#define ASSET_ATTR_Y "y"
#define ASSET_ATTR_Z "z"

namespace Dream {

  using namespace std;
class AssetDefinition {
    map<string,string> mAttributes;
    nlohmann::json mJson;
    string mProjectPath;
  public:
    AssetDefinition(nlohmann::json);
    ~AssetDefinition(void);

    pair<string,string> mapPair(string,string);

    void setUUID(string);
    string getUUID();
    void setName(string);
    string getName();
    void setType(string);
    string getType();
    void setFormat(string);
    string getFormat();

    string getNameAndUUIDString();

    void addAttribute(string,string);

    string  getAttribute         (string);
    int getAttributeAsInt    (string);
    float getAttributeAsFloat  (string);
    double getAttributeAsDouble (string);
    bool getAttributeAsBool   (string);
    long getAttributeAsLong   (string);
    char getAttributeAsChar   (string);

    string getAssetTypeDirectory();

    bool isTypeAnimation();
    bool isTypeAudio();
    bool isTypeModel();
    bool isTypeScript();
    bool isTypeShader();
    bool isTypePhysicsObject();
    bool isTypeLight();
    bool isTypeSprite();

    bool isScriptFormatChai();
    bool isModelFormatAssimp();
    bool isAudioFormatWav();
    bool isAudioFormatOgg();
    bool isAnimationFormatDream();

    string getAssetPath();

    void setProjectPath(string);
    string getProjectPath();

    void loadMetadata();
    nlohmann::json getJson();
    void showStatus();
  }; // End of AssetDefinition

} // End of Dream

#endif // End of ASSETDEFINITION_H

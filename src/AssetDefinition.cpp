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

namespace Dream {
  AssetDefinition::AssetDefinition(nlohmann::json jsonDef) {
    mJson = jsonDef;
    loadMetadata();
    showStatus();
  }

  AssetDefinition::~AssetDefinition(void) {}

  void AssetDefinition::setProjectPath(string path) {
    mProjectPath = path;
  }

  string AssetDefinition::getProjectPath() {
    return mProjectPath;
  }

  void AssetDefinition::loadMetadata() {
    // UUID
    if (mJson[ASSET_UUID].is_null()) {
      cerr << "AssetDefinition: Construction Asset from JSON with nullptr UUID." << endl;
    } else {
      setUUID(mJson[ASSET_UUID]);
    }
    // Name
    if (mJson[ASSET_NAME].is_null()) {
      cerr << "AssetDefinition: Construction Asset from JSON with nullptr Name." << endl;
    } else {
      setName(mJson[ASSET_NAME]);
    }
    // Type
    if (mJson[ASSET_TYPE].is_null()) {
      cerr << "AssetDefinition: Construction Asset from JSON with nullptr Type" << endl;
    } else {
      setType(mJson[ASSET_TYPE]);
    }
    // Format
    if (mJson[ASSET_FORMAT].is_null()) {
      cerr << "AssetDefinition: Construction Asset from JSON with nullptr Format" << endl;
    } else {
      setFormat(mJson[ASSET_FORMAT]);
    }
  }

  pair<string,string> AssetDefinition::mapPair(string key, string value) {
    return pair<string,string>(key,value);
  }

  void AssetDefinition::setUUID(string uuid) {
    addAttribute(ASSET_UUID, uuid);
  }

  void AssetDefinition::addAttribute(string key, string value) {
    mAttributes.insert(mapPair(key,value));
  }

  string AssetDefinition::getAttribute(string attribute) {
    try {
      return mAttributes.at(attribute);
    } catch (const exception &ex) {
      cerr << "AssetDefinition: No Attribute - " << attribute << endl;
      return "";
    }
  }

  int AssetDefinition::getAttributeAsInt(string attr) {
    return stoi(getAttribute(attr));
  }

  float AssetDefinition::getAttributeAsFloat(string attr) {
    return stof(getAttribute(attr));
  }

  double AssetDefinition::getAttributeAsDouble(string attr) {
    return stod(getAttribute(attr));
  }

  bool AssetDefinition::getAttributeAsBool(string attr) {
    return strcasecmp("true",getAttribute(attr).c_str()) == 0;
  }

  long AssetDefinition::getAttributeAsLong(string attr) {
    return stol(getAttribute(attr));
  }

  char AssetDefinition::getAttributeAsChar(string attr) {
    return getAttribute(attr).c_str()[0];
  }

  string AssetDefinition::getUUID() {
    return getAttribute(ASSET_UUID);
  }

  void AssetDefinition::setName(string name) {
    addAttribute(ASSET_NAME,name);
  }

  string AssetDefinition::getName() {
    return getAttribute(ASSET_NAME);
  }

  void AssetDefinition::setType(string type) {
    addAttribute(ASSET_TYPE,type);
  }

  string AssetDefinition::getType() {
    return getAttribute(ASSET_TYPE);
  }

  void AssetDefinition::setFormat(string format) {
    addAttribute(ASSET_FORMAT,format);
  }

  string AssetDefinition::getFormat() {
    return getAttribute(ASSET_FORMAT);
  }

  bool AssetDefinition::isTypeLight() {
    return getType().compare(ASSET_TYPE_LIGHT) == 0;
  }

  bool AssetDefinition::isTypePhysicsObject() {
    return getType().compare(ASSET_TYPE_PHYSICS_OBJECT) == 0;
  }

  bool AssetDefinition::isTypeAnimation() {
    return getType().compare(ASSET_TYPE_ANIMATION) == 0;
  }

  bool AssetDefinition::isTypeAudio() {
    return getType().compare(ASSET_TYPE_AUDIO) == 0;
  }

  bool AssetDefinition::isTypeModel() {
    return getType().compare(ASSET_TYPE_MODEL) == 0;
  }

  bool AssetDefinition::isTypeScript() {
    return getType().compare(ASSET_TYPE_SCRIPT) == 0;
  }

  bool AssetDefinition::isTypeShader() {
    return getType().compare(ASSET_TYPE_SHADER) == 0;
  }

  bool AssetDefinition::isTypeSprite() {
    return getType().compare(ASSET_TYPE_SPRITE) == 0;
  }

  bool AssetDefinition::isScriptFormatLua() {
    return getFormat().compare(ASSET_DEF_FORMAT_SCRIPT_LUA) == 0;
  }

  bool AssetDefinition::isModelFormatAssimp() {
    return getFormat().compare(ASSET_DEF_FORMAT_MODEL_ASSIMP) == 0;
  }

  bool AssetDefinition::isAudioFormatMusic() {
    return getFormat().compare(ASSET_DEF_FORMAT_AUDIO_MUSIC) == 0;
  }

  bool AssetDefinition::isAudioFormatSoundEffect() {
    return getFormat().compare(ASSET_DEF_FORMAT_AUDIO_SFX) == 0;
  }

  string AssetDefinition::getAssetTypeDirectory() {
    string type = getType();
    if (isTypeAnimation()) {
      return ASSET_DIR_ANIMATION;
    } else if (isTypeAudio()) {
      return ASSET_DIR_AUDIO;
    } else if (isTypeModel()) {
      return ASSET_DIR_MODEL;
    } else if (isTypeScript()) {
      return ASSET_DIR_SCRIPT;
    } else if (isTypeShader()) {
      return ASSET_DIR_SHADER;
    } else if (isTypeSprite()) {
      return ASSET_DIR_SPRITE;
    } else {
      return "Type Not Found";
    }
  }

  string AssetDefinition::getNameAndUUIDString() {
    return getName() + " (" + getUUID() + ")";
  }

  string AssetDefinition::getAssetPath() {
    stringstream pathStream;
    pathStream << DIR_PATH_SEP
               << ASSET_DIR << DIR_PATH_SEP
               << getAssetTypeDirectory() << DIR_PATH_SEP
               << getUUID() << DIR_PATH_SEP
               << getFormat();
    return pathStream.str();
  }

  void AssetDefinition::showStatus() {
    cout << "AssetDefinition:" << endl;
    for (const auto& any : mAttributes) {
      string value = any.second;
      cout << "\t" << any.first << " : " << value << endl;
    }
  }

  nlohmann::json AssetDefinition::getJson() {
    return mJson;
  }
} // End of Dream

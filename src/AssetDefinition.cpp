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
  AssetDefinition::AssetDefinition(nlohmann::json json) {
    mJson = json;
    loadMetadata();
    showStatus();
  }

  AssetDefinition::~AssetDefinition(void) {}

  void AssetDefinition::setProjectPath(std::string path) {
    mProjectPath = path;
  }

  std::string AssetDefinition::getProjectPath() {
    return mProjectPath;
  }

  void AssetDefinition::loadMetadata() {
    // UUID
    if (mJson[ASSET_UUID].is_null()) {
      std::cerr << "AssetDefinition: Construction Asset from JSON with NULL UUID." << std::endl;;
    } else {
      setUUID(mJson[ASSET_UUID]);
    }
    // Name
    if (mJson[ASSET_NAME].is_null()) {
      std::cerr << "AssetDefinition: Construction Asset from JSON with NULL Name." << std::endl;;
    } else {
      setName(mJson[ASSET_NAME]);
    }
    // Type
    if (mJson[ASSET_TYPE].is_null()) {
      std::cerr << "AssetDefinition: Construction Asset from JSON with NULL Type" << std::endl;;
    } else {
      setType(mJson[ASSET_TYPE]);
    }
    // Format
    if (mJson[ASSET_FORMAT].is_null()) {
      std::cerr << "AssetDefinition: Construction Asset from JSON with NULL Format" << std::endl;;
    } else {
      setFormat(mJson[ASSET_FORMAT]);
    }
  }

  std::pair<std::string,std::string> AssetDefinition::mapPair(std::string key, std::string value) {
    return std::pair<std::string,std::string>(key,value);
  }

  void AssetDefinition::setUUID(std::string uuid) {
    addAttribute(ASSET_UUID, uuid);
  }

  void AssetDefinition::addAttribute(std::string key, std::string value) {
    mAttributes.insert(mapPair(key,value));
  }

  std::string AssetDefinition::getAttribute(std::string attribute) {
    try {
      return mAttributes.at(attribute);
    } catch (const std::exception &ex) {
      std::cerr << "AssetDefinition: Unable to get attribute: " << attribute << std::endl;
      std::cerr << "\tException: " << ex.what() << std::endl;
      return "";
    }
  }

  int AssetDefinition::getAttributeAsInt(std::string attr) {
    return std::stoi(getAttribute(attr));
  }

  float AssetDefinition::getAttributeAsFloat(std::string attr) {
    return std::stof(getAttribute(attr));
  }

  double AssetDefinition::getAttributeAsDouble(std::string attr) {
    return std::stod(getAttribute(attr));
  }

  bool AssetDefinition::getAttributeAsBool(std::string attr) {
    return strcasecmp("true",getAttribute(attr).c_str()) == 0;
  }

  long AssetDefinition::getAttributeAsLong(std::string attr) {
    return std::stol(getAttribute(attr));
  }

  char AssetDefinition::getAttributeAsChar(std::string attr) {
    return getAttribute(attr).c_str()[0];
  }

  std::string AssetDefinition::getUUID() {
    return getAttribute(ASSET_UUID);
  }

  void AssetDefinition::setName(std::string name) {
    addAttribute(ASSET_NAME,name);
  }

  std::string AssetDefinition::getName() {
    return getAttribute(ASSET_NAME);
  }

  void AssetDefinition::setType(std::string type) {
    addAttribute(ASSET_TYPE,type);
  }

  std::string AssetDefinition::getType() {
    return getAttribute(ASSET_TYPE);
  }

  void AssetDefinition::setFormat(std::string format) {
    addAttribute(ASSET_FORMAT,format);
  }

  std::string AssetDefinition::getFormat() {
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

  bool AssetDefinition::isScriptFormatChai() {
    return getFormat().compare(ASSET_DEF_FORMAT_SCRIPT_CHAI) == 0;
  }

  bool AssetDefinition::isModelFormatAssimp() {
    return getFormat().compare(ASSET_DEF_FORMAT_MODEL_ASSIMP) == 0;
  }

  bool AssetDefinition::isAudioFormatWav() {
    return getFormat().compare(ASSET_DEF_FORMAT_AUDIO_WAV) == 0;
  }

  bool AssetDefinition::isAudioFormatOgg() {
    return getFormat().compare(ASSET_DEF_FORMAT_AUDIO_OGG) == 0;
  }

  bool AssetDefinition::isAnimationFormatDream() {
    return getFormat().compare(ASSET_DEF_FORMAT_ANIMATION_DREAM) == 0;
  }

  std::string AssetDefinition::getAssetTypeDirectory() {
    std::string type = getType();
    if (type.compare(ASSET_TYPE_ANIMATION)==0) {
      return ASSET_DIR_ANIMATION;
    } else if (type.compare(ASSET_TYPE_AUDIO)==0) {
      return ASSET_DIR_AUDIO;
    } else if (type.compare(ASSET_TYPE_MODEL)==0) {
      return ASSET_DIR_MODEL;
    } else if (type.compare(ASSET_TYPE_SCRIPT)==0) {
      return ASSET_DIR_SCRIPT;
    } else if (type.compare(ASSET_TYPE_SHADER) == 0) {
      return ASSET_DIR_SHADER;
    } else {
      return "Type Not Found";
    }
  }

  std::string AssetDefinition::getNameAndUUIDString() {
    return getName() + " (" + getUUID() + ")";
  }

  std::string AssetDefinition::getAssetPath() {
    std::stringstream pathStream;
    pathStream << DIR_PATH_SEP
               << ASSET_DIR << DIR_PATH_SEP
               << getAssetTypeDirectory() << DIR_PATH_SEP
               << getUUID() << DIR_PATH_SEP
               << getFormat();
    return pathStream.str();
  }

  void AssetDefinition::showStatus() {
    std::cout << "AssetDefinition:" << std::endl;
    for (const auto& any : mAttributes) {
      std::string value = any.second;
      std::cout << "\t" << any.first << " : " << value << std::endl;
    }
  }

  nlohmann::json AssetDefinition::getJson() {
    return mJson;
  }
} // End of Dream

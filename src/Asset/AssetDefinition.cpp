/*
* Dream::Asset::AssetDefinition
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

namespace Dream {
namespace Asset {
	
	AssetDefinition::AssetDefinition(void) {}
	AssetDefinition::~AssetDefinition(void) {}

	AssetDefinition::AssetDefinition(nlohmann::json json) {
		loadMetadata(json);
		if (isTypeCollisionShape()) {
			loadCollisionShapeAttributes(json);
		}
		showStatus();
	}
	
	void AssetDefinition::loadMetadata(nlohmann::json json) {
		if (json[ASSET_UUID].is_null()) {
			std::cerr << "AssetDefinition: Construction Asset from JSON with NULL UUID." << std::endl;;
		} else {
			setUUID(json[ASSET_UUID]);
		}
		
		if (json[ASSET_NAME].is_null()) {
			std::cerr << "AssetDefinition: Construction Asset from JSON with NULL Name." << std::endl;;
		} else {
			setName(json[ASSET_NAME]);
		}
		
		if (json[ASSET_TYPE].is_null()) {
			std::cerr << "AssetDefinition: Construction Asset from JSON with NULL Type" << std::endl;;
		} else {
			setType(json[ASSET_TYPE]);
		}
		
		if (json[ASSET_FORMAT].is_null()) {
			std::cerr << "AssetDefinition: Construction Asset from JSON with NULL Format" << std::endl;;
		} else {
			setFormat(json[ASSET_FORMAT]);
		}
	}
	
	void AssetDefinition::loadCollisionShapeAttributes(nlohmann::json json) {
		// Margin
		if (!json[ASSET_ATTR_MARGIN].is_null() && json[ASSET_ATTR_MARGIN].is_number()) {
			addAttribute(ASSET_ATTR_MARGIN, std::to_string((float)json[ASSET_ATTR_MARGIN]));
		}
		// Mass
		if (!json[ASSET_ATTR_MASS].is_null() && json[ASSET_ATTR_MASS].is_number()) {
			addAttribute(ASSET_ATTR_MASS, std::to_string((float)json[ASSET_ATTR_MASS]));
		}
		// Radius
		if (!json[ASSET_ATTR_RADIUS].is_null() && json[ASSET_ATTR_RADIUS].is_number()) {
			addAttribute(ASSET_ATTR_RADIUS, std::to_string((float)json[ASSET_ATTR_RADIUS]));
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
		} catch (std::exception ex) {
			std::cerr << "AssetDefinition: Unable to get attribute: " << attribute << std::endl;
			std::cerr << "Exception: " << ex.what() << std::endl;
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
	
	bool AssetDefinition::isTypeCollisionShape() {
		return getType().compare(ASSET_TYPE_COLLISION_SHAPE) == 0;
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
		std::cout << "AssetDefinition: Showing Definition..." << std::endl;
		for (const auto& any : mAttributes) {
			std::string value = any.second;
			std::cout << "\t" << any.first << " : " << value << std::endl;
		}
	}
	
} // End of AssetDefinition
} // End of Dream

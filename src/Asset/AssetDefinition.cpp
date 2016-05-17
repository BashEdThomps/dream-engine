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

	std::pair<std::string,std::string> AssetDefinition::mapPair(std::string key, std::string value) {
		return std::pair<std::string,std::string>(key,value);
	}
		
	void AssetDefinition::setUUID(std::string uuid) {
		mAttributes.insert(mapPair(ASSET_UUID, uuid));
	}

	std::string AssetDefinition::getUUID() {
		return mAttributes.at(ASSET_UUID);
	}

	void AssetDefinition::setName(std::string name) {
		mAttributes.insert(mapPair(ASSET_NAME,name));
	}

	std::string AssetDefinition::getName() {
		return mAttributes.at(ASSET_NAME);
	}

	void AssetDefinition::setType(std::string type) {
		mAttributes.insert(mapPair(ASSET_TYPE,type));
	}

	std::string AssetDefinition::getType() {
		return mAttributes.at(ASSET_TYPE);
	}

	void AssetDefinition::setFormat(std::string format) {
		mAttributes.insert(mapPair(ASSET_FORMAT,format));
	}

	std::string AssetDefinition::getFormat() {
		return mAttributes.at(ASSET_FORMAT);
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
		
	bool AssetDefinition::isScriptFormatJavaScript() {
		return getFormat().compare(ASSET_DEF_FORMAT_SCRIPT_JAVASCRIPT) == 0;
	}
	
	bool AssetDefinition::isScriptFormatChaiScript() {
		return getFormat().compare(ASSET_DEF_FORMAT_SCRIPT_CHAISCRIPT) == 0;
	}
		
	bool AssetDefinition::isModelFormatWaveFront() {
		return getFormat().compare(ASSET_DEF_FORMAT_MODEL_WAVEFRONT) == 0;
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
		
} // End of AssetDefinition
} // End of Dream

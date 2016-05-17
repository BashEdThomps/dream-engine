/*
* Dream::Asset::Asset
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

#include "Asset.h"

namespace Dream {
namespace Asset {
	Asset::Asset(void) {}
	Asset::~Asset(void) {}

	Asset::Asset(nlohmann::json json) {
		if (json[ASSET_UUID].is_null()) {
			std::cerr << "Asset: Construction Asset from JSON with NULL UUID." << std::endl;;
		} else {
			setUUID(json[ASSET_UUID]);
		}
		
		if (json[ASSET_NAME].is_null()) {
			std::cerr << "Asset: Construction Asset from JSON with NULL Name." << std::endl;;
		} else {
			setName(json[ASSET_NAME]);
		}
		
		if (json[ASSET_TYPE].is_null()) {
			std::cerr << "Asset: Construction Asset from JSON with NULL Type" << std::endl;;
		} else {
			setType(json[ASSET_TYPE]);
		}
		
		if (json[ASSET_FORMAT].is_null()) {
			std::cerr << "Asset: Construction Asset from JSON with NULL Format" << std::endl;;
		} else {
			setFormat(json[ASSET_FORMAT]);
		}
	}

	std::pair<std::string,std::string> Asset::mapPair(std::string key, std::string value) {
		return std::pair<std::string,std::string>(key,value);
	}
		
	void Asset::setUUID(std::string uuid) {
		mAttributes.insert(mapPair(ASSET_UUID, uuid));
	}

	std::string Asset::getUUID() {
		return mAttributes.at(ASSET_UUID);
	}

	void Asset::setName(std::string name) {
		mAttributes.insert(mapPair(ASSET_NAME,name));
	}

	std::string Asset::getName() {
		return mAttributes.at(ASSET_NAME);
	}

	void Asset::setType(std::string type) {
		mAttributes.insert(mapPair(ASSET_TYPE,type));
	}

	std::string Asset::getType() {
		return mAttributes.at(ASSET_TYPE);
	}

	void Asset::setFormat(std::string format) {
		mAttributes.insert(mapPair(ASSET_FORMAT,format));
	}

	std::string Asset::getFormat() {
		return mAttributes.at(ASSET_FORMAT);
	}
		
} // End of Asset
} // End of Dream

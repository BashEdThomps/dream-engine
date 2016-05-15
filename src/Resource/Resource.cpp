/*
* Dream::Resource::Resource
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

#include "Resource.h"

namespace Dream {
namespace Resource {
	Resource::Resource(void) {}
	Resource::~Resource(void) {}

	Resource::Resource(nlohmann::json json) {
		if (json[RESOURCE_UUID].is_null()) {
			std::cerr << "Resource: Construction Resource from JSON with NULL UUID." << std::endl;;
		} else {
			setUUID(json[RESOURCE_UUID]);
		}
		
		if (json[RESOURCE_NAME].is_null()) {
			std::cerr << "Resource: Construction Resource from JSON with NULL Name." << std::endl;;
		} else {
			setName(json[RESOURCE_NAME]);
		}
		
		if (json[RESOURCE_TYPE].is_null()) {
			std::cerr << "Resource: Construction Resource from JSON with NULL Type" << std::endl;;
		} else {
			setType(json[RESOURCE_TYPE]);
		}
		
		if (json[RESOURCE_FORMAT].is_null()) {
			std::cerr << "Resource: Construction Resource from JSON with NULL Format" << std::endl;;
		} else {
			setFormat(json[RESOURCE_FORMAT]);
		}
	}

	std::pair<std::string,std::string> Resource::mapPair(std::string key, std::string value) {
		return std::pair<std::string,std::string>(key,value);
	}
		
	void Resource::setUUID(std::string uuid) {
		mAttributes.insert(mapPair(RESOURCE_UUID, uuid));
	}

	std::string Resource::getUUID() {
		return mAttributes.at(RESOURCE_UUID);
	}

	void Resource::setName(std::string name) {
		mAttributes.insert(mapPair(RESOURCE_NAME,name));
	}

	std::string Resource::getName() {
		return mAttributes.at(RESOURCE_NAME);
	}

	void Resource::setType(std::string type) {
		mAttributes.insert(mapPair(RESOURCE_TYPE,type));
	}

	std::string Resource::getType() {
		return mAttributes.at(RESOURCE_TYPE);
	}

	void Resource::setFormat(std::string format) {
		mAttributes.insert(mapPair(RESOURCE_FORMAT,format));
	}

	std::string Resource::getFormat() {
		return mAttributes.at(RESOURCE_FORMAT);
	}
		
} // End of Resource
} // End of Dream

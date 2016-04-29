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
		Resource::Resource(void) {
		}

		Resource::Resource(nlohmann::json json) {
			mUUID = json [RESOURCE_JSON_UUID];
			mName = json [RESOURCE_JSON_NAME];
		}

		void Resource::setUUID(std::string uuid) {
			mUUID = uuid;
		}

		std::string Resource::getUUID() {
			return mUUID;
		}

		void Resource::setName(std::string name) {
			mName = name;
		}

		std::string Resource::getName() {
			return mName;
		}
	} // End of Resource
} // End of Dream

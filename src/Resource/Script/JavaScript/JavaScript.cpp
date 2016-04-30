/*
* Dream::Resource::Script::JavaScript
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

#include "JavaScript.h"

namespace Dream {
	namespace Resource {
		namespace Script {
			namespace JavaScript {

				JavaScript::JavaScript(nlohmann::json json) : Dream::Resource::Script::Script(json) {
				}

				JavaScript::~JavaScript(void) {
				}

				void JavaScript::generateAbsolutePaths(std::string projectDir, std::string projectUUID) {
					mScriptPath = projectDir          + PATH_SEP +
										    RESOURCE_DIR        + PATH_SEP +
										    RESOURCE_DIR_SCRIPT + PATH_SEP +
										    getUUID()           + PATH_SEP +
										    RESOURCE_FORMAT_JAVASCRIPT;
					std::cout << "Generated path for JavaScript resource: " << mScriptPath << std::endl;
				}

				ResourceInstance* JavaScript::createInstance() {
					return NULL;
				}
			} // End JavaScript
		} // End of Script
	} // End of Resource
} // End of Dream

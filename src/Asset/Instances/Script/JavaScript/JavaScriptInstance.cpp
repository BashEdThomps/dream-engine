/*
* Dream::Asset::Script::JavaScript
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

#include "JavaScriptInstance.h"

namespace Dream {
namespace Asset {
namespace Instance {
namespace Script {
namespace JavaScript {
	
	JavaScriptInstance::JavaScriptInstance(Dream::Asset::AssetDefinition* definition) :
		Dream::Asset::AssetInstance(definition) {}
	
	JavaScriptInstance::~JavaScriptInstance(void) {}
	
	bool JavaScriptInstance::load(std::string projectPath) {
		return false;
	}
	
} // End JavaScript
} // End of Script
} // End of Instance
} // End of Asset
} // End of Dream

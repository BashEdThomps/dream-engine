/*
* Dream::Asset::Script::Script
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

#ifndef JAVASCRIPT_H
#define JAVASCRIPT_H

#define ASSET_FORMAT_JAVASCRIPT "js"

#include "../../AssetInstance.h"

namespace Dream {
namespace Asset {
namespace Instance {
namespace Script {
namespace JavaScript {
	
	class JavaScriptInstance : public Dream::Asset::Instance::AssetInstance {
	public:
		JavaScriptInstance(Dream::Asset::AssetDefinition*);
		~JavaScriptInstance(void);
	}; // End of JavaScript
	
} // End of JavaScript
} // End of Script
} // End of Instance
} // End of Asset
} // End of Dream

#endif // End of SCRIPT_H

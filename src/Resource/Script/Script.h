/*
* Dream::Resource::Script::Script
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

#ifndef SCRIPT_H
#define SCRIPT_H

#include "../Resource.h"

namespace Dream {
	namespace Resource {
		namespace Script {
			class Script : public Dream::Resource::Resource {
			public:
				Script(nlohmann::json);
				~Script(void);
			}; // End of Script
		} // End of Script
	} // End of Resource
} // End of Dream

#endif // End of SCRIPT_H

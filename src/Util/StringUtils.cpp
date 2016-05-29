/*
* StringUtils
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

#include "StringUtils.h"

namespace Dream {
	namespace Util {

		StringUtils::StringUtils(void) {
		}

		StringUtils::~StringUtils(void) {
		}

		std::string StringUtils::indent(int level) {
			std::string retval = "";
			for (int i=0;i<level;i++) {
				retval += "  ";
			}
			retval += "* ";
			return retval;
		}
		
		std::string StringUtils::boolToYesNo(bool value) {
			return (value ? "Yes" : "No");
		}
	} // End Util
}   // End of Dream

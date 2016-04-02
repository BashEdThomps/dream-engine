/*
* Resource
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

#ifndef RESOURCE_H
#define RESOURCE_H

#include <iostream>

namespace Dream {
	namespace Resource {
		class Resource {
			std::string mName;
		public:
			Resource(void);
			~Resource(void);

			void setName(std::string);
			std::string getName();

		//protected:
		//private:
		}; // End of Resource
	}
} // End of Dream

#endif // End of RESOURCE_H

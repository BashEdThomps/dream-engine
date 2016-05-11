/*
* Dream::Interface
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

#ifndef INTERFACE_H
#define INTERFACE_H

namespace Dream {
	class Interface {
	public:
		Interface(void);
		virtual ~Interface(void) {}
		virtual bool init() = 0;
		virtual void update() = 0;
	}; // End of Interface
} // End of Dream

#endif // End of INTERFACE_H

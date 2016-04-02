/*
* JSONFileReader
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

#ifndef JSONFILEREADER_H
#define JSONFILEREADER_H

#include <iostream>

namespace Dream {
	class JSONFileReader {
	private:
		std::string mPath;
	public:
		JSONFileReader(std::string);
		~JSONFileReader(void);
	//protected:
	//private:
	}; // End of JSONFileReader
} // End of Dream

#endif // End of JSONFILEREADER_H

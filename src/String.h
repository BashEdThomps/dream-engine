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

#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <iostream>
#include <vector>
#include <sstream>

namespace Dream {
  class String {
  public:
    String(void);
    ~String(void);
    static std::string indent(int);
    static std::string boolToYesNo(bool);
    static std::string floatVectorToString(std::vector<float>);
  }; // End of StringUtils
}      // End of Dream

#endif // End of STRINGUTILS_H

/*
* Dream::Util::UUID
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

#include "UUID.h"
#include <cstdio>
#include <cstdlib>

namespace Dream {

  UUID::UUID()  {
    return;
  }

  UUID::~UUID() {
     return;
  }

  string UUID::generateUUID() {
    size_t bufSize = sizeof(char)*20;
    char* buffer = static_cast<char*>(malloc(bufSize));
    snprintf(
      buffer, bufSize,
      "%2x%2x-%2x%2x-%2x%2x-%2x%2x",
      rand()%255, rand()%255,
      rand()%255,rand()%255,
      rand()%255,rand()%255,
      rand()%255,rand()%255
    );
    string retval = string(buffer);
    free(buffer);
    return retval;
  }

} // End of Dream

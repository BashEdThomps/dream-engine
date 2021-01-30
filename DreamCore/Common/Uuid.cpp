/*
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

#include "Uuid.h"
#include <cstdio>
#include <cstdlib>
#include <chrono>

using std::srand;
using std::chrono::high_resolution_clock;

namespace octronic::dream
{
    const UuidType Uuid::INVALID = 0;

    UuidType Uuid::generateUuid()
    {
        srand(static_cast<unsigned int>(high_resolution_clock::now().time_since_epoch().count()));
        UuidType retval = INVALID;
        while (retval == INVALID)
        {
           retval = static_cast<UuidType>(rand());
        }
        return retval;
    }
}

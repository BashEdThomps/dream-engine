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


#include "Environment.h"

#include <sys/types.h>
#ifdef WIN32

#else
#include <unistd.h>
#include <pwd.h>
#endif

using std::string;

namespace Dream
{
    string Environment::getHomeDirectory()
    {
        #ifdef WIN32
        return "";
        #else
        char *homedir;
        if ((homedir = getenv("HOME")) == nullptr)
        {
            homedir = getpwuid(getuid())->pw_dir;
        }
        return string(homedir);
        #endif
    }
}

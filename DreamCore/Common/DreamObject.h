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


#pragma once

#include <string>
#include "Constants.h"

using namespace std;
using namespace spdlog;

namespace Dream
{
    class DreamObject
    {
    public:
        DreamObject(string classname);
        virtual ~DreamObject();

        void setLogClassName(string);
        string getClassName();

    protected:
        shared_ptr<logger> getLog() const;
        string  _CLASSNAME_;
    };
}

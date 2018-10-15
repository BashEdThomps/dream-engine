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


#include "DreamObject.h"

#include <iostream>

using std::cerr;
using std::endl;

namespace Dream
{
    DreamObject::DreamObject
    (string classname)
        : _CLASSNAME_(classname)
    {}

    DreamObject::~DreamObject
    ()
    {}

    void
    DreamObject::setLogClassName
    (string name)
    {
       _CLASSNAME_ = name;

    }

    string
    DreamObject::getClassName
    ()
    {
        return _CLASSNAME_;
    }

    std::shared_ptr<spdlog::logger>
    DreamObject::getLog
    ()
    const
    {
        try
        {
            auto log = spdlog::get(_CLASSNAME_);
            if (log == nullptr)
            {
                log = spdlog::stdout_color_mt(_CLASSNAME_);
            }
            return log;
        }
        catch (spdlog::spdlog_ex ex)
        {
           cerr << "********** Exception getting log for " << _CLASSNAME_ <<" **********"
                << endl << ex.what() << endl;
           return nullptr;
        }
    }
}

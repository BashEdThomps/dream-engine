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
#ifdef DREAM_LOG
    using namespace spdlog;
#endif

namespace Dream
{
    /**
     * @brief The base class for objects in Dream. This class provides access to
     * Constants and logging functionality to any DreamObject.
     */
    class DreamObject
    {


    public:
        /**
         * @param classname Logging class name.
         */
        DreamObject
        (const string& classname)
#ifdef DREAM_LOG
            : _CLASSNAME_(classname)
#endif
        {}

        /**
         * @brief Default destructor.
         */
        virtual ~DreamObject
        ()
        {}

#ifdef DREAM_LOG
        /**
         * @brief Override the currently defined class name used during logging.
         * @param className The new class name to log with.
         */
        inline void
        setLogClassName
        (const string& name)
        {
            _CLASSNAME_ = name;

        }

        /**
         * @return Get the class name used for logging.
         */
        inline
        string
        getClassName
        ()
        const
        {
            return _CLASSNAME_;
        }

    protected:
        /**
         * @return Get the instance of the class's logger. Instances are shared
         * between objects with the same _CLASSNAME_.
         */
        inline
        std::shared_ptr<spdlog::logger>
        getLog
        ()
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

        /**
         * @brief Name to use during logging.
         */
        string  _CLASSNAME_;
#endif
    };
}

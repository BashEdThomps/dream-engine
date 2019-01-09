/*
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#pragma once

#include "../deps/json/json.hpp"

#include <string>

#include "../Components/Transform.h"
#include "DreamObject.h"

using std::string;
using nlohmann::json;

namespace Dream
{
    /**
     * @brief Definition is an abstract class that provides naming, id-ing and
     * json serialization functionality.
     */
    class Definition : public DreamObject
    {
    protected:
        /**
         * @brief Internal JSON structure that defines the object.
         */
        json mJson;

    public:
        /**
         * @brief Default constructor.
         * @param data Set the internal mJson variable to this data.
         */
        inline Definition
        (json data) : DreamObject("Definition"), mJson(data) {}

        virtual inline ~Definition() {}

        /**
         * @brief Get the current json object describing this object
         * @return JSON data object.
         */
        virtual inline json
        getJson
        ()
        {
            return mJson;
        }

        /**
         * @param name Name to compare.
         * @return True if this Definition has the given name.
         */
        inline bool
        hasName
        (const string& uuid)
        {
            string s = mJson[Constants::NAME];
            return s.compare(uuid) == 0;
        }

        /**
         * @return The Definition's name.
         */
        inline string
        getName
        ()
        {
            if (mJson[Constants::NAME].is_null())
            {
                mJson[Constants::NAME] = "";
            }
            return mJson[Constants::NAME];
        }

        /**
         * @param name The name to give the Definition.
         */
        inline void
        setName
        (const string& name)
        {
            mJson[Constants::NAME] = name;
        }

        /**
         * @param uuid uuid to compare.
         * @return True if this Definition has the give uuid.
         */
        inline bool
        hasUuid
        (const string& uuid)
        {
            string s = mJson[Constants::UUID];
            return s.compare(uuid) == 0;
        }

        /**
         * @return The Definition's uuid.
         */
        inline string
        getUuid
        ()
        {
            if (mJson[Constants::UUID].is_null())
            {
                mJson[Constants::UUID] = "";
            }
            return mJson[Constants::UUID];
        }

        /**
         * @param uuid Uuid to give the Definition;
         */
        inline void
        setUuid
        (const string& uuid)
        {
            mJson[Constants::UUID] = uuid;
        }

        /**
         * @return A combined name and uuid string in the following format:
         * "[ NAME : UUID ]"
         */
        inline string
        getNameAndUuidString
        ()
        {
            return "[" + getName() + " : " + getUuid() + "]";
        }
    };
}

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

#include "LockableObject.h"

#include "Constants.h"
#include "Components/Transform.h"
#include "Math.h"
#include "Uuid.h"

#include <sstream>
#include <json.hpp>

using nlohmann::json;
using std::stringstream;

namespace Dream
{
    /**
     * @brief Definition is an abstract class that provides naming, id-ing and
     * json serialization functionality.
     */
    class Definition : public LockableObject
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
        (const json& data) : LockableObject(), mJson(data)
        {
            if (mJson[Constants::UUID].is_string())
            {
               mJson[Constants::UUID] = UuidTools::generateUuid();
            }
        }

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
        (const string& name)
        {
            string s = mJson[Constants::NAME];
            return s.compare(name) == 0;
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
        (uint32_t uuid)
        {
            uint32_t s = mJson[Constants::UUID];
            return s == uuid;
        }

        /**
         * @return The Definition's uuid.
         */
        inline uint32_t
        getUuid
        ()
        {
            if (mJson[Constants::UUID].is_null())
            {
                mJson[Constants::UUID] = UuidTools::generateUuid();
            }
            return mJson[Constants::UUID];
        }

        /**
         * @param uuid Uuid to give the Definition;
         */
        inline void
        setUuid
        (uint32_t uuid)
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
            stringstream ss;
            ss << "[" << getName() << " : " << getUuid() << "]";
            return ss.str();
        }

        json
        inline wrapVector3(const Vector3& v)
        {
           json retval = json::object();
           retval[Constants::X] = v.x();
           retval[Constants::Y] = v.y();
           retval[Constants::Z] = v.z();
           return retval;
        }

        Vector3
        inline unwrapVector3
        (const json& j)
        {
            return Vector3
            (
                j[Constants::X],
                j[Constants::Y],
                j[Constants::Z]
            );
        }
    };
}

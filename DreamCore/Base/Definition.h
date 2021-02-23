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



#include "Common/Constants.h"
#include "Math/Transform.h"
#include "Common/Uuid.h"

#include <sstream>
#include <json.hpp>

using nlohmann::json;
using std::stringstream;

namespace octronic::dream
{
    /**
     * @brief Definition is an abstract class that provides Name, ID and
     * JSON serialization functions.
     */
    class Definition
    {


    public:
        /**
         * @brief Default constructor.
         * @param data Set the internal mJson variable to this data.
         * @param className name of the class
         */
        Definition(const string& className, const json& data);
        virtual  ~Definition();

        /**
         * @brief Get the current json object describing this object
         * @return JSON data object.
         */
        virtual json getJson();

        /**
         * @param name Name to compare.
         * @return True if this Definition has the given name.
         */
        bool hasName(const string& name);

        /**
         * @return The Definition's name.
         */
        string getName();

        /**
         * @param name The name to give the Definition.
         */
        void setName(const string& name);

        /**
         * @param uuid uuid to compare.
         * @return True if this Definition has the give uuid.
         */
        bool hasUuid(UuidType uuid);

        /**
         * @return The Definition's uuid.
         */
        UuidType getUuid();

        /**
         * @param uuid Uuid to give the Definition;
         */
        void setUuid(UuidType uuid);

        /**
         * @return A combined name and uuid string in the following format:
         * "[ NAME : UUID ]"
         */
        string getNameAndUuidString();

    protected:
        /**
         * @brief Internal JSON structure that defines the object.
         */
        json mJson;
    };
}

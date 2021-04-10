#pragma once

#include "Math/Transform.h"
#include "Common/Uuid.h"

#include <sstream>
#include <json.hpp>
#include <memory>

using std::string;
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
        Definition(const json& data) ;


        Definition(const Definition&) = delete;
        Definition& operator=(const Definition&) = delete;

        Definition(Definition&&) = default;
        Definition& operator=(Definition&&) = default;

        bool operator==(const Definition& other) const;
        bool operator!=(const Definition& other) const;

        bool operator==(const UuidType& uuid) const;
        bool operator!=(const UuidType& uuid) const;

        /**
         * @brief Get the current json object describing this object
         * @return JSON data object.
         */
        virtual json getJson();
        void setJson(const json&);

        /**
         * @param name Name to compare.
         * @return True if this Definition has the given name.
         */
        bool hasName(const string& name) const;

        /**
         * @return The Definition's name.
         */
        string getName() const;

        /**
         * @param name The name to give the Definition.
         */
        void setName(const string& name);

        /**
         * @param uuid uuid to compare.
         * @return True if this Definition has the give uuid.
         */
        bool hasUuid(UuidType uuid) const;

        /**
         * @return The Definition's uuid.
         */
        UuidType getUuid() const;

        /**
         * @param uuid Uuid to give the Definition;
         */
        void setUuid(UuidType uuid);

        /**
         * @return A combined name and uuid string in the following format:
         * "[ NAME : UUID ]"
         */
        string getNameAndUuidString() const;

        void setGroup(const string& group);
        string getGroup() const;


    protected:
        /**
         * @brief Internal JSON structure that defines the object.
         */
        json mJson;
    };
}

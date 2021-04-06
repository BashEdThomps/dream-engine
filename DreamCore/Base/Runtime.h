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

#include <string>
#include <memory>

#include "Definition.h"
#include "RuntimeTasks.h"

using std::string;
using std::reference_wrapper;

namespace octronic::dream
{
    class Definition;
    /**
     * @brief Abstract class that contains Runtime data for DreamObjects that
     * are used to implement a Project.
     */
    class Runtime
    {
    public:
        /**
         * @brief Default Constructor
         * @param def Definition from which the Runtime was instanciated.
         */
        Runtime(Definition& def);

        Runtime(Runtime&&) = default;
        Runtime& operator=(Runtime&&) = default;

        Runtime(const Runtime&) = delete;
        Runtime& operator=(const Runtime&) = delete;

        // Comparison Operator
        bool operator==(const Runtime& other);

        /**
         * @brief Default destructor.
         */
        virtual ~Runtime();

        /**
         * @return The Runtime's UUID - Same as it's Definition, unless specified
         * to be random, mDefinition was null.
         */
        UuidType getUuid()const;
        /**
         * @return The Runtime's UUID as a std::string (used for dictionaries in
         * scripting).
         */
        string getUuidString()const;

        void setUuid(UuidType uuid);

        bool hasUuid(UuidType uuid) const;

        /**
         * @return The Runtime's Name - same as it's Definition.
         */
        string getName() const;

        void setName(const string& name);

        bool hasName(const string& name) const;

        string getNameAndUuidString() const;

        /**
         * @brief Use information from the give Definition to create the Runtime
         * object's initial state.
         * @return Success/Failure.
         */
        virtual bool loadFromDefinition() = 0;

        /**
         * @return The Definition from which this Runtime was Instanciated.
         */
        Definition& getDefinition() const;
    protected:

        /**
         * @brief UUID of this Runtime, given by it's Definition.
         */
        UuidType mUuid;
        /**
         * @brief Name of this Runtime, given by it's Definition.
         */
        string mName;

    private:
        /**
         * @brief Definition from which this runtime was instanciated. Owned by
         * ProejctRuntime. This type is reference_wrapper<> to satisfy
         * Cpp17MoveInsertable.
         */
        reference_wrapper<Definition> mDefinition;
    };
}

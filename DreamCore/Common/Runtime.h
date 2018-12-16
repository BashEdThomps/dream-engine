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

#include "DreamObject.h"

using std::string;

namespace Dream
{
    class Definition;

    /**
     * @brief Abstract class that contains Runtime data for DreamObjects that
     * are used to implement a Project.
     */
    class Runtime : public DreamObject
    {
    protected:
        /**
         * @brief Definition from which this runtime was instanciated.
         */
        Definition* mDefinition;
        /**
         * @brief UUID of this Runtime, given by it's Definition.
         */
        string mUuid;
        /**
         * @brief Name of this Runtime, given by it's Definition.
         */
        string mName;
    public:
        /**
         * @brief Default Constructor
         * @param definition Definition from which the Runtime was instanciated.
         */
        Runtime(Definition* definition);
        /**
         * @brief Default destructor.
         */
        virtual ~Runtime();

        /**
         * @return The Runtime's UUID - same as it's Definition.
         */
        string getUuid();
        void setUuid(string uuid);
        bool hasUuid(string uuid);

        /**
         * @return The Runtime's Name - same as it's Definition.
         */
        string getName();
        void setName(string name);
        bool hasName(string name);

        string getNameAndUuidString();

        /**
         * @brief Use information from the give Definition to create the Runtime
         * object's initial state.
         * @return Success/Failure.
         */
        virtual bool useDefinition() = 0;

        /**
         * @brief Delete anything managed by this runtime that was instanciated
         * but no longer needed .
         */
        virtual void collectGarbage() = 0;

        /**
         * @return The Definition from which this Runtime was Instanciated.
         */
        Definition* getDefinition();
    };
}

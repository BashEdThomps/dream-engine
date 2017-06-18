/*
 * Runtime.h
 *
 * Created: 17 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
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

using std::string;

namespace Dream
{
    class Runtime
    {
    protected:
        string mUuid;
        string mName;
    public:
        Runtime(string uuid = "", string name = "");
        virtual ~Runtime();

        string getUuid() const;
        void setUuid(const string &uuid);
        bool hasUuid(const string &uuid);

        string getName() const;
        void setName(const string &name);
        bool hasName(const string &name);

        string getNameAndUuidString();
    };
}

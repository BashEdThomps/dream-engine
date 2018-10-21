/*
 * Definition.h
 *
 * Created: 16 2017 by Ashley
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

#include <json.hpp>

#include <string>

#include "../Components/Transform3D.h"
#include "DreamObject.h"

using std::string;
using nlohmann::json;

namespace Dream
{
    class IDefinition : public DreamObject
    {
    protected:
        json mJson;
    public:
        IDefinition(json data);
        virtual ~IDefinition();

        virtual json getJson();
        virtual void showStatus() = 0;

        bool hasName(string name);
        string getName();
        void setName(string name);

        bool hasUuid(string uuid);
        string getUuid();
        void setUuid(string uuid);

        string getNameAndUuidString();
    };
}

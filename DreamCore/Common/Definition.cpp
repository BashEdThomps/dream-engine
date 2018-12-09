/*
 * Definition.cpp
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

#include "Definition.h"

namespace Dream
{
    Definition::Definition
    (json data)
        : // Init list
          DreamObject("Definition"),
          mJson(data)
    {

    }

    Definition::~Definition
    ()
    {
    }

    json
    Definition::getJson
    ()
    {
        return mJson;
    }


    bool
    Definition::hasUuid
    (string uuid)
    {
        string s = mJson[Constants::UUID];
        return s.compare(uuid) == 0;
    }

    string
    Definition::getUuid
    ()
    {
        if (mJson[Constants::UUID].is_null())
        {
            mJson[Constants::UUID] = "";
        }
        return mJson[Constants::UUID];
    }

    string
    Definition::getName
    ()
    {
        if (mJson[Constants::NAME].is_null())
        {
            mJson[Constants::NAME] = "";
        }
        return mJson[Constants::NAME];
    }

    bool
    Definition::hasName
    (string uuid)
    {
        string s = mJson[Constants::NAME];
        return s.compare(uuid) == 0;
    }

    string
    Definition::getNameAndUuidString
    ()
    {
        return "[" + getName() + " : " + getUuid() + "]";
    }

    void
    Definition::setName(string name)
    {
        mJson[Constants::NAME] = name;
    }

    void
    Definition::setUuid(string uuid)
    {
        mJson[Constants::UUID] = uuid;
    }
}



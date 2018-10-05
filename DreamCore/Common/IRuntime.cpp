/*
 * Runtime.cpp
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
#include "IRuntime.h"

namespace Dream
{
    IRuntime::IRuntime
    (
        IDefinition* def,
        string uuid,
        string name
    )
        : // Init List
          DreamObject("IRuntime"),
          mDefinition(def),
          mUuid(uuid),
          mName(name)
    {

    }

    IRuntime::~IRuntime
    ()
    {

    }

    string
    IRuntime::getUuid
    ()
    {
        return mUuid;
    }

    void
    IRuntime::setUuid
    (string uuid)
    {
        mUuid = uuid;
    }

    string
    IRuntime::getName
    ()
    {
        return mName;
    }

    void
    IRuntime::setName
    (string name)
    {
        mName = name;
    }

    string
    IRuntime::getNameAndUuidString
    ()
    {
        return "[" + getName() + " : " + getUuid() + "]";
    }

    bool
    IRuntime::hasName
    (string name)
    {
        return getName().compare(name) == 0;
    }

    bool
    IRuntime::hasUuid
    (string uuid)
    {
        return getUuid().compare(uuid) == 0;
    }

    IDefinition*
    IRuntime::getDefinition
    ()
    {
        return mDefinition;
    }

}

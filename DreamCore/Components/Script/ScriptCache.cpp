/*
 * Created: 04/04/2017 2017 by Ashley
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

#include "ScriptCache.h"

#include "ScriptDefinition.h"
#include "ScriptRuntime.h"
#include "Common/Logger.h"
#include "Common/File.h"
#include "TaskManager/TaskManager.h"
#include "Project/ProjectRuntime.h"

namespace Dream
{
    ScriptCache::ScriptCache
    (ProjectRuntime* runtime)
        : Cache (runtime)
    {
        LOG_TRACE("ScriptCache: Constructing");
    }

    ScriptCache::~ScriptCache
    ()
    {
        LOG_TRACE("ScriptCache: Destructing");
    }

    SharedAssetRuntime*
    ScriptCache::loadRuntime
    (AssetDefinition* def)
    {
        auto scriptDef = static_cast<ScriptDefinition*>(def);
        auto newScript = new ScriptRuntime(scriptDef,mProjectRuntime);
        auto absPath = getAbsolutePath(scriptDef);
        File scriptFile(absPath);
        newScript->setSource(scriptFile.readString());
        newScript->useDefinition();
        mRuntimes.push_back(newScript);
        return newScript;
    }
}

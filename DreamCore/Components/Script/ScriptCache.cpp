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

#include "ScriptRuntime.h"
#include "ScriptDefinition.h"
#include "ScriptRuntime.h"
#include "Project/ProjectRuntime.h"
#include "Common/Logger.h"
#include "Components/Storage/StorageManager.h"
#include "Components/Storage/File.h"

using std::unique_lock;

namespace octronic::dream
{
    ScriptCache::ScriptCache
    (ProjectRuntime* runtime)
        : Cache ("ScriptCache",runtime)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("Constructing");
    }

    ScriptCache::~ScriptCache
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("Destructing");
    }

    SharedAssetRuntime* ScriptCache::loadRuntime(AssetDefinition* def)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        ScriptDefinition* scriptDef = static_cast<ScriptDefinition*>(def);
        for (SharedAssetRuntime* inst : mRuntimes)
        {
            if (inst->getUuid() == scriptDef->getUuid())
            {
                return inst;
            }
        }
        string absPath = getAbsolutePath(scriptDef);
        StorageManager* sm = mProjectRuntime->getStorageManager();
        File* scriptFile = sm->openFile(absPath);



        ScriptRuntime* newScript = new ScriptRuntime(scriptDef,mProjectRuntime);

        if (!scriptFile->exists())
        {
            LOG_ERROR("ScriptCache: Script file does not exist");
            newScript->setSource("");
        }
        else
        {
        	newScript->setSource(scriptFile->readString());
        }

        if (!newScript->useDefinition())
        {
         	delete newScript;
            newScript = nullptr;
            sm->closeFile(scriptFile);
            scriptFile = nullptr;
        	LOG_ERROR("ScriptCache: Error Loading Script\n{}\n",newScript->getSource());
        }
        else
        {
        	LOG_TRACE("ScriptCache: Loaded Script Source\n{}\n",newScript->getSource());
        	mRuntimes.push_back(newScript);
            sm->closeFile(scriptFile);
            scriptFile = nullptr;
        }
        return newScript;
    }
}

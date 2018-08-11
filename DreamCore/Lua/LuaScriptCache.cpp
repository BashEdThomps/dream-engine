/*
 * LuaScriptCache.cpp
 *
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

#include "LuaScriptCache.h"

#include "../Utilities/FileReader.h"

namespace Dream
{
    LuaScriptCache::LuaScriptCache
    () : ILoggable ("LuaScriptCache")
    {
        auto log = getLog();
        log->trace("Constructing");
    }

    LuaScriptCache::~LuaScriptCache
    ()
    {
        auto log = getLog();
        log->trace("Destructing");
    }

    string
    LuaScriptCache::getScript
    (string path)
    {
        auto log = getLog();
      for (pair<string,string> it : mScriptCache)
      {
         if (it.first == path)
         {
             log->info("Found script in cache {}", path);
             return it.second;
         }
      }
      return readIntoCache(path);
    }

    string
    LuaScriptCache::readIntoCache
    (string path)
    {
        auto log = getLog();
        FileReader reader(path);
        if(!reader.readIntoString())
        {
            log->error("Error reading Lua script into cache from path '{}'", path);
           return "";
        }
        log->info("Inserting script ", path);
        string content = reader.getContentsAsString();
        mScriptCache.insert(pair<string,string>(path,content));
        return content;
    }
}

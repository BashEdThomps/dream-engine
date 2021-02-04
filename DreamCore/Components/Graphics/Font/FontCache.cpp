/*
 * FontCache.cpp
 *
 * Created: 12 2017 by Ashley
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

#include "FontCache.h"

#include "FontRuntime.h"
#include "FontDefinition.h"

#include "Components/Storage/File.h"
#include "Components/Storage/StorageManager.h"
#include "Project/ProjectRuntime.h"



namespace octronic::dream
{

    FontCache::FontCache(ProjectRuntime* pr)
        : Cache("FontCache",pr),
          mFreeTypeLibrary(nullptr)

    {
        initFreetypeLibrary();
    }

    FontCache::~FontCache()
    {
        FT_Done_FreeType(mFreeTypeLibrary);
    }

    bool FontCache::initFreetypeLibrary()
    {
        if(dreamTryLock()) {
            dreamLock();
            LOG_TRACE("FontCache: {}",__FUNCTION__);
            FT_Error ft_error = FT_Init_FreeType(&mFreeTypeLibrary);
            if( ft_error )
            {
                LOG_ERROR("FontManager: Unable to initialise FreeType Library: {}",ft_error);
                return false;
            }
            return true;
        } dreamElseLockFailed
    }

    SharedAssetRuntime* FontCache::loadRuntime(AssetDefinition* definition)
    {
        if(dreamTryLock()) {
            dreamLock();
            // Check Definition
            if (definition == nullptr)
            {
                LOG_ERROR("FontCache: Cannot load Font, definition is null");
                return nullptr;
            }

            // Create Runtime
            FontDefinition* fontDef = static_cast<FontDefinition*>(definition);
            FontRuntime* fontRuntime = new FontRuntime(fontDef, mProjectRuntime);
            if (!fontRuntime->useDefinition())
            {
                LOG_ERROR("FontCache: Error loading Font, useDefinition failed");
                delete fontRuntime;
                return nullptr;
            }
            else
            {
                mRuntimes.push_back(fontRuntime);
            }
            return fontRuntime;
        } dreamElseLockFailed
    }

    FT_Library FontCache::getFreeTypeLibrary()
    {
        return mFreeTypeLibrary;
    }

    const char* FontCache::getFreetypeErrorMessage(FT_Error err)
    {
        LOG_TRACE("FontManager: {}",__FUNCTION__);
#undef FTERRORS_H_
#define FT_ERRORDEF( e, v, s )  case e: return s;
#define FT_ERROR_START_LIST     switch (err) {
#define FT_ERROR_END_LIST       }
#include FT_ERRORS_H
        return "(Unknown error)";
    }
}

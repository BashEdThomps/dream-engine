/*
 * FontCache.h
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

#pragma once

#include <memory>
#include <map>

#include "../../../AssetDefinition.h"
#include "FontCharacter.h"
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;

namespace Dream
{
    class FontCache
    {
    private:
        static unique_ptr<FT_Library> mFreeTypeLib;
        static map<AssetDefinition*,map<GLchar, FontCharacter>> mCache;
        static map<GLchar, FontCharacter> generateCharMap(AssetDefinition*, FT_Face* face);
    public:
        static void initFreeTypeLib();
        static void destroyFreeTypeLib();
        static FT_Library* getFreeTypeLib();

        static void clearCache();
        static map<GLchar, FontCharacter> getCharMap(AssetDefinition*, FT_Face* face);
    };
}

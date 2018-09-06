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

#include <GL/glew.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../../../Common/DreamObject.h"

using std::shared_ptr;
using std::map;

namespace Dream
{
    class FontDefinition;
    class FontCharacter;

    class FontCache : public DreamObject
    {

    private:
        const static char CHAR_MAP_START;
        const static char CHAR_MAP_END;
        shared_ptr<FT_Library> mFreeTypeLib;
        map<shared_ptr<FontDefinition>, map<GLchar, FontCharacter>> mCache;
        map<GLchar, FontCharacter> generateCharMap(shared_ptr<FontDefinition>, shared_ptr<FT_Face> face);

    public:
        FontCache();
        ~FontCache();

        shared_ptr<FT_Library> getFreeTypeLib();
        map<GLchar, FontCharacter> getCharMap(shared_ptr<FontDefinition>, shared_ptr<FT_Face> face);

    };
}

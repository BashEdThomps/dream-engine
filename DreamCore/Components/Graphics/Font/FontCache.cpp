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

#include <iostream>
#include <algorithm>

#include "FontCharacter.h"
#include "FontDefinition.h"
#include "../../../Common/Constants.h"
#include <memory>

using std::unique_ptr;

namespace Dream
{
    const char FontCache::CHAR_MAP_START = 0x20;
    const char FontCache::CHAR_MAP_END = 0x7F;

    FontCache::FontCache
    () : DreamObject ("FontCache"),
        mFreeTypeLib(nullptr)
    {
        auto log = getLog();
        log->trace("Constructing");

        if (mFreeTypeLib == nullptr)
        {
            log->info("Initialising FreeType");

            mFreeTypeLib = new FT_Library();
            if (FT_Init_FreeType(mFreeTypeLib))
            {
                log->error("FontCache:: Fatal Error! Could not initialise FreeType library");
                mFreeTypeLib = nullptr;
            }
        }
    }

    FontCache::~FontCache
    ()
    {
        auto log = getLog();
        // Close Lib
        if (mFreeTypeLib != nullptr)
        {
            log->trace("Destroying FreeType");
            FT_Done_FreeType(*mFreeTypeLib);
            mFreeTypeLib = nullptr;
        }

        // Clear Cache
        for (auto definitionMapPair : mCache)
        {
            FontDefinition* fd = definitionMapPair.first;
            for (auto glCharFontCharMap : definitionMapPair.second)
            {
                FontCharacter fChar = glCharFontCharMap.second;
                log->info(
                    "Deleting GL texture {} for character {} of font {}",
                    fChar.TextureID, glCharFontCharMap.first ,fd->getNameAndUuidString()
                );
                glDeleteTextures(1, &fChar.TextureID);
            }
            definitionMapPair.second.clear();
        }
        mCache.clear();
    }

    map<GLchar,FontCharacter>
    FontCache::getCharMap
    (FontDefinition* definition, FT_Face* face)
    {
        auto log = getLog();
        auto it = mCache.find(definition);
        if (it != end(mCache))
        {
            log->info("Found {} in cache", definition->getNameAndUuidString());
            return mCache[definition];
        }
        else
        {
            log->info("Generating New Font {}",definition->getNameAndUuidString());
            return generateCharMap(definition,face);
        }
    }


    FT_Library*
    FontCache::getFreeTypeLib
    ()
    const
    {
        return mFreeTypeLib;
    }

    map<GLchar,FontCharacter>
    FontCache::generateCharMap
    (FontDefinition* definition, FT_Face* fontFace)
    {
        auto log = getLog();
        log->info("Generating Character Map...");

        map<GLchar, FontCharacter> charMap;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

        for (GLubyte c = CHAR_MAP_START; c < CHAR_MAP_END; c++)
        {
            log->info("FreeType: load Glyph for char {}", c);

            // Load character glyph
            if (FT_Load_Char(*fontFace, c, FT_LOAD_RENDER))
            {
                log->error("FreeType: Failed to load Glyph for char {}",c);
                continue;
            }

            // Generate texture
            GLuint texture;
            glGenTextures(1, &texture);

            log->info("Using Texture {}" ,texture);

            // Bind current texture
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D
                    (
                        GL_TEXTURE_2D, // Target texture
                        0,             // Level of detail (mipmap)
                        GL_RED,        // InternalFormat Number of colour components
                        static_cast<GLsizei>((*fontFace)->glyph->bitmap.width), // Width
                        static_cast<GLsizei>((*fontFace)->glyph->bitmap.rows),  // Height
                        0, // Border
                        GL_RED, // Format of pixel data
                        GL_UNSIGNED_BYTE, // Type of pixel data
                        (*fontFace)->glyph->bitmap.buffer // Data
                        );

            log->info("Char Texture Buffered");

            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);//GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);//GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            log->info("Texture options set");

            // Now store character for later use
            FontCharacter character = FontCharacter(
                        texture,
                        ivec2((*fontFace)->glyph->bitmap.width, (*fontFace)->glyph->bitmap.rows),
                        ivec2((*fontFace)->glyph->bitmap_left, (*fontFace)->glyph->bitmap_top),
                        static_cast<GLuint>((*fontFace)->glyph->advance.x)
                        );
            charMap.insert(pair<GLchar, FontCharacter>(c, character));

            log->info("Texture inserted into map");
        }

        FT_Done_Face(*fontFace);

        log->info("Finished Generating Character Map.");

        mCache.insert(pair<FontDefinition*,map<GLchar,FontCharacter>>(definition,charMap));
        return charMap;
    }

} // End Dream

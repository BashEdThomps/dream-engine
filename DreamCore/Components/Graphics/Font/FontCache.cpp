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

namespace Dream
{
    const char FontCache::CHAR_MAP_START = 0x20;
    const char FontCache::CHAR_MAP_END = 0x7F;

    FontCache::FontCache
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "FontCahce: Constructing" << endl;
        }

        if (mFreeTypeLib == nullptr)
        {
            if (Constants::DEBUG)
            {
                cout << "FontCache: Initialising FreeType" << endl;
            }

            mFreeTypeLib.reset(new FT_Library());
            if (FT_Init_FreeType(mFreeTypeLib.get()))
            {
                cerr << "FontCache:: Fatal Error! Could not initialise FreeType library" << endl;
                mFreeTypeLib.reset();
            }
        }
    }

    FontCache::~FontCache
    ()
    {
        if (mFreeTypeLib)
        {
            if (Constants::DEBUG)
            {
                cout << "FontCache: Destroying FreeType" << endl;
            }
            FT_Done_FreeType(*mFreeTypeLib.get());
            mFreeTypeLib.release();
        }

        for (auto definitionMapPair : mCache)
        {
            FontDefinition* fdHandle = definitionMapPair.first;
            for (auto glCharFontCharMap : definitionMapPair.second)
            {
               FontCharacter fChar = glCharFontCharMap.second;
               if (Constants::VERBOSE)
               {
                   cout << "FontCache: Deleting GL texture " << fChar.TextureID
                        << " for character " << glCharFontCharMap.first
                        << " of font " << fdHandle->getNameAndUuidString()
                        << endl;
               }
               glDeleteTextures(1, &fChar.TextureID);
            }
        }
    }

    map<GLchar,FontCharacter>
    FontCache::getCharMap
    (FontDefinition* definition, FT_Face* face)
    {
        auto it = mCache.find(definition);
        if (it != end(mCache))
        {
            if (Constants::DEBUG)
            {
                cout << "FontCache: Found"
                     << definition->getNameAndUuidString()
                     << " in font cache."
                     << endl;
            }
            return mCache[definition];
        }
        else
        {
            if (Constants::DEBUG)
            {
                cout << "FontCache: Generating New Font "
                     << definition->getNameAndUuidString()
                     << endl;
            }
            return generateCharMap(definition,face);
        }
    }


    FT_Library*
    FontCache::getFreeTypeLib
    ()
    {
        return mFreeTypeLib.get();
    }

    map<GLchar,FontCharacter>
    FontCache::generateCharMap
    (FontDefinition* definition, FT_Face* fontFace)
    {
        if (Constants::DEBUG)
        {
            cout << "FontCache: Generating Character Map..." << endl;
        }

        map<GLchar, FontCharacter> charMap;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

        for (GLubyte c = CHAR_MAP_START; c < CHAR_MAP_END; c++)
        {
            if (Constants::DEBUG)
            {
                cout << "FontCache: FreeType: load Glyph for char " << c << endl;
            }

            // Load character glyph
            if (FT_Load_Char(*fontFace, c, FT_LOAD_RENDER))
            {
                cerr << "FontCache: FreeType: Failed to load Glyph for char "
                     << c
                     << endl;
                continue;
            }

            // Generate texture
            GLuint texture;
            glGenTextures(1, &texture);

            if (Constants::DEBUG)
            {
                cout << "FontCache: Using Texture " << texture << endl;
            }

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

            if (Constants::DEBUG)
            {
                cout << "FontCache: Char Texture Buffered" << endl;
            }

            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);//GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);//GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            if (Constants::DEBUG)
            {
                cout << "FontCache: Texture options set" << endl;
            }

            // Now store character for later use
            FontCharacter character = FontCharacter(
                texture,
                ivec2((*fontFace)->glyph->bitmap.width, (*fontFace)->glyph->bitmap.rows),
                ivec2((*fontFace)->glyph->bitmap_left, (*fontFace)->glyph->bitmap_top),
                static_cast<GLuint>((*fontFace)->glyph->advance.x)
            );
            charMap.insert(pair<GLchar, FontCharacter>(c, character));

            if (Constants::DEBUG)
            {
                cout << "FontCache: Texture inserted into map" << endl;
            }
        }

        FT_Done_Face(*fontFace);

        if (Constants::DEBUG)
        {
            cout << "FontCache: Finished Generating Character Map." << endl;
        }

        mCache.insert(pair<FontDefinition*,map<GLchar,FontCharacter>>(definition,charMap));
        return charMap;
    }

} // End Dream

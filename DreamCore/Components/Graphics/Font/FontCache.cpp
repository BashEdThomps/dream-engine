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
#include "../../../Common/Constants.h"

namespace Dream
{
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
    }

    map<GLchar,FontCharacter>
    FontCache::getCharMap
    (AssetDefinition* definition, FT_Face* face)
    {
        auto it = mCache.find(definition);
        if (it != end(mCache))
        {
            return mCache[definition];
        }
        else
        {
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
    (AssetDefinition* definition, FT_Face* fontFace)
    {
        if (Constants::DEBUG)
        {
            cout << "FontCache: Generating Character Map..." << endl;
        }

        map<GLchar, FontCharacter> charMap;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

        for (GLubyte c = 0; c < 128; c++)
        {
            //cout << "FontCache: FreeType: load Glyph for char " << c << endl;
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
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                        GL_TEXTURE_2D,
                        0,
                        GL_RED,
                        static_cast<GLsizei>((*fontFace)->glyph->bitmap.width),
                        static_cast<GLsizei>((*fontFace)->glyph->bitmap.rows),
                        0,
                        GL_RED,
                        GL_UNSIGNED_BYTE,
                        (*fontFace)->glyph->bitmap.buffer
                        );

            //cout << "FontCache: Char Texture Buffered" << endl;

            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            //cout << "FontCache: Texture options set" << endl;
            // Now store character for later use
            FontCharacter character =
            {
                texture,
                glm::ivec2((*fontFace)->glyph->bitmap.width, (*fontFace)->glyph->bitmap.rows),
                glm::ivec2((*fontFace)->glyph->bitmap_left, (*fontFace)->glyph->bitmap_top),
                static_cast<GLuint>((*fontFace)->glyph->advance.x)
            };
            charMap.insert(std::pair<GLchar, FontCharacter>(c, character));

            //cout << "FontCache: Texture inserted into map" << endl;
        }

        FT_Done_Face(*fontFace);

        if (Constants::DEBUG)
        {
            cout << "FontCache: Finished Generating Character Map." << endl;
        }

        mCache.insert(pair<AssetDefinition*,map<GLchar,FontCharacter>>(definition,charMap));
        return charMap;
    }

} // End Dream

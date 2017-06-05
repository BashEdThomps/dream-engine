/*
 * FontInstance
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

#include "FontInstance.h"
#include <glm/glm.hpp>

namespace Dream
{

    FT_Library* FontInstance::sFreeTypeLib = nullptr;

    FontInstance::FontInstance
    (AssetDefinition* definition, Transform3D* transform)
        : IAssetInstance(definition,transform)
    {
        ftInit();
        mColour = {1,1,1};
        mFontFace = nullptr;
        setText("NO TEXT SET");
    }

    void
    FontInstance::ftInit
    ()
    {

        if (sFreeTypeLib == nullptr)
        {
             if (DEBUG)
            {
                cout << "FontInstance: Initialising FreeType" << endl;
            }

            sFreeTypeLib = new FT_Library();
            if (FT_Init_FreeType(sFreeTypeLib))
            {
                cerr << "FontInstance:: Fatal Error! Could not initialise FreeType library" << endl;
                delete sFreeTypeLib;
                sFreeTypeLib = nullptr;
            }
        }
    }

    void
    FontInstance::ftDestroy
    ()
    {
        if (sFreeTypeLib)
        {
            if (DEBUG)
            {
                cout << "FontInstance: Destroying FreeType" << endl;
            }
            FT_Done_FreeType(*sFreeTypeLib);
            delete sFreeTypeLib;
            sFreeTypeLib = nullptr;
        }
    }

    FontInstance::~FontInstance
    ()
    {
        if (DEBUG)
        {
            cout << "FontInstance: Destroying Object" << endl;
        }

        return;
    }

    bool
    FontInstance::load
    (string projectPath)
    {
        string path = projectPath+mDefinition->getAssetPath();
        string directory = path.substr(0, path.find_last_of('/'));
        if (DEBUG)
        {
            cout << "FontInstance: Loading font from " << path << endl;
        }

        if (sFreeTypeLib)
        {

            mFontFace = new FT_Face();
            if (FT_New_Face(*sFreeTypeLib,path.c_str(),0,mFontFace))
            {
                cerr << "FontInstance: Unable to create font. Error calling FT_New_Face" << endl;
            }
        }
        else
        {
            cerr << "FontInstance: Cannot instanciate font, FreeTypeLib == nullptr" << endl;
            mLoaded = false;
            return false;
        }


        loadExtraAttributes(mDefinition->toJson());

        mLoaded = true; //mFont != nullptr;
        return mLoaded;
    }

    void
    FontInstance::loadExtraAttributes
    (nlohmann::json jsonData)
    {
        float red = jsonData[FONT_COLOUR][FONT_RED];
        float green = jsonData[FONT_COLOUR][FONT_GREEN];
        float blue = jsonData[FONT_COLOUR][FONT_BLUE];

        setColour(red,green,blue);

        mSize = jsonData[FONT_SIZE];

        if (mFontFace)
        {
            FT_Set_Pixel_Sizes(
                *mFontFace,0,
                static_cast<FT_UInt>(mSize)
            );
        }

        if (DEBUG)
        {
            cout << "FontInstance" << endl
                 << "\tr: "    << red   << endl
                 << "\tg: "    << green << endl
                 << "\tb: "    << blue  << endl
                 << "\tSize: " << mSize << endl
                 << endl;
        }

        generateCharacterMap();

        if (DEBUG)
        {
            cout << "FontInstance: finished loading extra attributes" << endl;
        }
        return;
    }

    FT_Face*
    FontInstance::getFontFace
    ()
    {
        return mFontFace;
    }

    void
    FontInstance::setText
    (string text)
    {
        mText = text;
    }

    string
    FontInstance::getText
    ()
    {
        return mText;
    }


    int
    FontInstance::getWidth
    ()
    {
        return 1;//mSurface->w;
    }

    int
    FontInstance::getHeight
    ()
    {
        return 1;//mSurface->h;
    }


    void
    FontInstance::setColour
    (float red, float green, float blue)
    {
        mColour[0] = red;
        mColour[1] = green;
        mColour[2] = blue;
    }

    // TODO - Optimise to store one charset per font definition rather than per instance
    void
    FontInstance::generateCharacterMap
    ()
    {
        if (DEBUG)
        {
            cout << "FontInstance: Generating Character Map..." << endl;
        }

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

        for (GLubyte c = 0; c < 128; c++)
        {
            //cout << "FontInstance: FreeType: load Glyph for char " << c << endl;
            // Load character glyph
            if (FT_Load_Char(*mFontFace, c, FT_LOAD_RENDER))
            {
                cerr << "FontInstance: FreeType: Failed to load Glyph for char "
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
                        static_cast<GLsizei>((*mFontFace)->glyph->bitmap.width),
                        static_cast<GLsizei>((*mFontFace)->glyph->bitmap.rows),
                        0,
                        GL_RED,
                        GL_UNSIGNED_BYTE,
                        (*mFontFace)->glyph->bitmap.buffer
            );

            //cout << "FontInstance: Char Texture Buffered" << endl;

            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            //cout << "FontInstance: Texture options set" << endl;
            // Now store character for later use
            Character character =
            {
                texture,
                glm::ivec2((*mFontFace)->glyph->bitmap.width, (*mFontFace)->glyph->bitmap.rows),
                glm::ivec2((*mFontFace)->glyph->bitmap_left, (*mFontFace)->glyph->bitmap_top),
                static_cast<GLuint>((*mFontFace)->glyph->advance.x)
            };
            mCharacterMap.insert(std::pair<GLchar, Character>(c, character));

            //cout << "FontInstance: Texture inserted into map" << endl;
        }

        FT_Done_Face(*mFontFace);
        if (DEBUG)
        {
            cout << "FontInstance: Finished Generating Character Map." << endl;
        }
    }

    map<GLchar,Character>
    FontInstance::getCharacterMap
    ()
    {
        return mCharacterMap;
    }

    vector<float>
    FontInstance::getColour
    ()
    {
       return mColour;
    }
} // End Dream

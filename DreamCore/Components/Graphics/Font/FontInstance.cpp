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

#include "FontCache.h"
#include "FontInstance.h"
#include "FontCharacter.h"

#include "FontDefinition.h"
#include "../../../Scene/SceneObject/SceneObjectRuntime.h"

namespace Dream
{

    FontInstance::FontInstance
    (FontCache* cache, FontDefinition* definition, SceneObjectRuntime* transform)
        : IAssetInstance(definition,transform),
          mCacheHandle(cache)

    {
        mColour = {1,1,1};
        mFontFace = nullptr;
        setText("NO TEXT SET");
    }

    FontInstance::~FontInstance
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "FontInstance: Destroying Object" << endl;
        }

        return;
    }

    bool
    FontInstance::load
    (string projectPath)
    {
        string path = projectPath + mDefinitionHandle->getAssetPath();
        string directory = path.substr(0, path.find_last_of('/'));

        if (Constants::DEBUG)
        {
            cout << "FontInstance: Loading font from " << path << endl;
        }

        if (mCacheHandle->getFreeTypeLib())
        {

            mFontFace.reset(new FT_Face());
            if (FT_New_Face(*mCacheHandle->getFreeTypeLib(),path.c_str(),0,mFontFace.get()))
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


        loadExtraAttributes(mDefinitionHandle->getJson());

        mLoaded = true; //mFont != nullptr;
        return mLoaded;
    }

    void
    FontInstance::loadExtraAttributes
    (nlohmann::json jsonData)
    {
        float red = jsonData[Constants::FONT_COLOUR][Constants::RED];
        float green = jsonData[Constants::FONT_COLOUR][Constants::GREEN];
        float blue = jsonData[Constants::FONT_COLOUR][Constants::BLUE];

        setColour(red,green,blue);

        mSize = jsonData[Constants::FONT_SIZE];

        if (mFontFace)
        {
            FT_Set_Pixel_Sizes(
                        *mFontFace,0,
                        static_cast<FT_UInt>(mSize)
                        );
        }

        if (Constants::DEBUG)
        {
            cout << "FontInstance: Red: " << red
                 << " Green: " << green
                 << " Blue: " << blue
                 << " Size: " << mSize
                 << endl;
        }

        mCacheHandle->getCharMap(dynamic_cast<FontDefinition*>(mDefinitionHandle),mFontFace.get());

        if (Constants::DEBUG)
        {
            cout << "FontInstance: Finished loading extra attributes" << endl;
        }
        return;
    }

    FT_Face*
    FontInstance::getFontFace
    ()
    {
        return mFontFace.get();
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

    vector<float>
    FontInstance::getColour
    ()
    {
        return mColour;
    }

    map<GLchar,FontCharacter>
    FontInstance::getCharMap
    ()
    {
        return mCacheHandle->getCharMap
        (
            dynamic_cast<FontDefinition*>(mDefinitionHandle),
            mFontFace.get()
        );
    }
} // End Dream

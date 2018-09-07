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
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "FontCache.h"
#include "FontInstance.h"
#include "FontCharacter.h"

#include "FontDefinition.h"
#include "../../../Scene/SceneObject/SceneObjectRuntime.h"

namespace Dream
{

    FontInstance::FontInstance
    (shared_ptr<FontCache> cache, shared_ptr<FontDefinition> definition, shared_ptr<SceneObjectRuntime> transform)
        : IAssetInstance(definition,transform),
          mCache(cache)

    {
        setLogClassName("FontInstance");
        mColour = {1,1,1};
        mFontFace = nullptr;
        setText("NO_TEXT_SET");
    }

    FontInstance::~FontInstance
    ()
    {
        auto log = getLog();
        log->info("Destroying Object" );

        return;
    }

    bool
    FontInstance::load
    (string projectPath)
    {
        auto log = getLog();
        string path = projectPath + mDefinition->getAssetPath();
        string directory = path.substr(0, path.find_last_of('/'));

            log->info("Loading font from ", path );

        if (mCache->getFreeTypeLib())
        {

            mFontFace.reset(new FT_Face());
            if (FT_New_Face(*mCache->getFreeTypeLib(),path.c_str(),0,mFontFace.get()))
            {
                log->error("Unable to create font. Error calling FT_New_Face" );
            }
        }
        else
        {
            log->error("Cannot instanciate font, FreeTypeLib == nullptr" );
            mLoaded = false;
            return false;
        }


        loadExtraAttributes(mDefinition->getJson());

        mLoaded = true; //mFont != nullptr;
        return mLoaded;
    }

    void
    FontInstance::loadExtraAttributes
    (nlohmann::json jsonData)
    {
        auto log = getLog();
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

        log->info("FontInstance: Red: {}\nGreen: {}\nBlue: {}\nSize: {}\n",red,green,blue,mSize);
        mCache->getCharMap(dynamic_pointer_cast<FontDefinition>(mDefinition),mFontFace);
        log->info("FontInstance: Finished loading extra attributes" );
        return;
    }

    shared_ptr<FT_Face>
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
        mColour[Constants::RED_INDEX] = red;
        mColour[Constants::GREEN_INDEX] = green;
        mColour[Constants::BLUE_INDEX] = blue;
    }

    vector<float>
    FontInstance::getColour
    ()
    {
        return mColour;
    }

    vec3
    FontInstance::getColourAsVec3
    ()
    {
        return vec3
        (
            mColour[Constants::RED_INDEX],
            mColour[Constants::GREEN_INDEX],
            mColour[Constants::BLUE_INDEX]
        );
    }

    map<GLchar,FontCharacter>
    FontInstance::getCharMap
    ()
    {
        return mCache->getCharMap
        (
            dynamic_pointer_cast<FontDefinition>(mDefinition),
            mFontFace
        );
    }
} // End Dream

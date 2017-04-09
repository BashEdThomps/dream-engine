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

namespace Dream {

    FontInstance::FontInstance(
            AssetDefinition* definition,
            Transform3D* transform) : AssetInstance(definition,transform) {
        mChanged = false;
        mSurface = nullptr;
        setText("NO TEXT SET");
        loadExtraAttributes(mDefinition->getJson());
    }

    FontInstance::~FontInstance() {

        if (DEBUG) {
            cout << "FontInstance: Destroying Object" << endl;
        }

        if (mSurface != nullptr) {
            SDL_FreeSurface(mSurface);
        }
        if (mFont != nullptr) {
            TTF_CloseFont(mFont);
        }
        return;
    }

    bool FontInstance::load(string projectPath) {
        string path = projectPath+mDefinition->getAssetPath();
        string directory = path.substr(0, path.find_last_of('/'));
        if (DEBUG) {
            cout << "FontInstance: Loading font from " << path << endl;
        }
        mFont = TTF_OpenFont(path.c_str(),mSize);
        mLoaded = mFont != nullptr;
        return mLoaded;
    }

    void FontInstance::loadExtraAttributes(nlohmann::json jsonData) {
        mSize = jsonData[FONT_SIZE];
        int red = jsonData[FONT_COLOUR][FONT_RED];
        int green = jsonData[FONT_COLOUR][FONT_GREEN];
        int blue = jsonData[FONT_COLOUR][FONT_BLUE];
        if (DEBUG) {
            cout << "FontInstance: Setting Colour"
                 << " r:" << red
                 << " g:" << green
                 << " b:" << blue
                 << endl;
        }
        setColour(red,green,blue);
        return;
    }

    TTF_Font* FontInstance::getFont() {
        return mFont;
    }

    void FontInstance::setText(string text) {
        mChanged = true;
        mText = text;
    }

    string FontInstance::getText() {
        return mText;
    }

    void FontInstance::renderToTexture() {

        if (mSurface != nullptr) {
          SDL_FreeSurface(mSurface);
        }

        mSurface = SDL_ConvertSurfaceFormat(
            TTF_RenderUTF8_Solid(mFont, mText.c_str(), mColour),
            SDL_PIXELFORMAT_RGBA8888, 0
        );

        // Create Texture
        glGenTextures(1, &mTexture);
        glBindTexture(GL_TEXTURE_2D, mTexture);

        glTexImage2D(
          GL_TEXTURE_2D, 0, GL_RGBA8, mSurface->w, mSurface->h, 0,
          GL_RGBA, GL_UNSIGNED_BYTE, mSurface->pixels
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        mChanged = false;
    }

    GLuint FontInstance::getTexture() {
        return mTexture;
    }

    int FontInstance::getWidth() {
        return mSurface->w;
    }

    int FontInstance::getHeight() {
        return mSurface->h;
    }

    bool FontInstance::hasChanged() {
        return mChanged;
    }

   void FontInstance::setColour(Uint8 red, Uint8 green, Uint8 blue) {
    mColour.r = red;
    mColour.g = green;
    mColour.b = blue;
    mChanged = true;
   }

} // End Dream

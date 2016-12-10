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
    setText("_NO_TEXT_SET_");
    loadExtraAttributes(mDefinition->getJson());
  }

  FontInstance::~FontInstance() {
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
    cout << "FontInstance: Loading font from " << path << endl;
    mFont = TTF_OpenFont(path.c_str(),mSize);
    return mFont != nullptr;
  }

  void FontInstance::loadExtraAttributes(nlohmann::json jsonData) {
    mSize = jsonData[FONT_SIZE];
    int red = jsonData[FONT_COLOUR][FONT_RED];
    int green = jsonData[FONT_COLOUR][FONT_GREEN];
    int blue = jsonData[FONT_COLOUR][FONT_BLUE];
    mColour = {
      (unsigned char)red,
      (unsigned char)green,
      (unsigned char)blue
    };
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
    mSurface = TTF_RenderText_Blended(mFont, mText.c_str(), mColour);
    GLint colors = mSurface->format->BytesPerPixel;
    GLint texture_format;
    // Determine colour format
    if (colors == 4) {
      texture_format = GL_RGBA;
    } else {
      texture_format = GL_RGB;
    }
    // Create Texture
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexImage2D(
      GL_TEXTURE_2D, 0, texture_format, mSurface->w, mSurface->h, 0,
      (GLuint)texture_format, GL_UNSIGNED_BYTE, mSurface->pixels
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

} // End Dream

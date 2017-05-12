/*
 * FontInstance
 *
 * Copyright 2016 Octronic. All rights reserved.
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

#ifndef FONTINSTANCE_H
#define FONTINSTANCE_H

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../../IAssetInstance.h"
#include "../../Constants.h"

namespace Dream {
  class FontInstance : public IAssetInstance {
  private:
    SDL_Surface* mSurface;
    int mSize;
    TTF_Font *mFont;
    SDL_Color mColour;
    string mText;
    GLuint mTexture;
    bool mChanged;
  public:
    FontInstance(AssetDefinition*,Transform3D*);
    ~FontInstance();
    bool load(string);
    void loadExtraAttributes(nlohmann::json);
    TTF_Font* getFont();
    void setText(string);
    string getText();
    void setColour(Uint8,Uint8,Uint8);
    void setSize(int);
    void renderToTexture();
    GLuint getTexture();
    int getWidth();
    int getHeight();
    bool hasChanged();
    void exposeLuaApi(lua_State*);

  }; // End of FontInstance
} // End of Dream

#endif // FONTINSTANCE_H

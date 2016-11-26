/*
 * SpriteInstance.cpp
 *
 * Created: 25/11/2016 2016 by Ashley
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
#include "SpriteInstance.h"
#include "SDL2/SDL_image.h"

namespace Dream {

  SpriteInstance::SpriteInstance(
      AssetDefinition* definition,
      Transform3D* transform) : AssetInstance(definition,transform) {
    loadExtraAttributes(mDefinition->getJson());
  }

  SpriteInstance::~SpriteInstance() {}

  bool SpriteInstance::load(string projectPath) {

    if (mRenderer == nullptr) {
      cerr << "SpriteInstance: Renderer has not been set!" << endl;
      return false;
    }

    string path = projectPath+mDefinition->getAssetPath();
    mTexture = IMG_LoadTexture(mRenderer, path.c_str());

    if (mTexture == nullptr){
      cerr << "SpriteInstance: Unable to load image" << endl;
      return false;
    }

    mDestination = new SDL_Rect();

    mDestination->x = static_cast<int>(
          mTransform->getTranslation()[TRANSFORM_X]
          );

    mDestination->y = static_cast<int>(
          mTransform->getTranslation()[TRANSFORM_Y]
          );

    SDL_QueryTexture(mTexture, NULL, NULL, &mSpriteWidth, &mSpriteHeight);

    // Default to full sprite tile size if not specified
    if (mTileWidth == 0 && mTileHeight == 0) {
      mTileWidth = mSpriteWidth;
      mTileHeight = mSpriteHeight;
    }

    return mTexture != nullptr;
  }

  void SpriteInstance::loadExtraAttributes(nlohmann::json jsonData) {
    mTileWidth = static_cast<int>(
      jsonData[SPRITE_TILE_SIZE][SPRITE_TILE_WIDTH]
    );
    mTileHeight = static_cast<int>(
      jsonData[SPRITE_TILE_SIZE][SPRITE_TILE_HEIGHT]
    );
  }

  SDL_Rect* SpriteInstance::getDestination() {
    return mDestination;
  }

  SDL_Texture *SpriteInstance::getTexture() {
    return mTexture;
  }

} // End Dream

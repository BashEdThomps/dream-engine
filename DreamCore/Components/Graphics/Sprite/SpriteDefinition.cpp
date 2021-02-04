/*
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
#include "SpriteDefinition.h"



namespace octronic::dream
{
    SpriteDefinition::SpriteDefinition
    (ProjectDefinition* pd, const json &js)
        :AssetDefinition("SpriteDefinition",pd,js)
    {
    }

    void SpriteDefinition::setWidth(int width)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::ASSET_ATTR_SPRITE_WIDTH] = width;

        } dreamElseLockFailed
    }

    int SpriteDefinition::getWidth()
    {
        if(dreamTryLock()) {
            dreamLock();

            int width = 1;
            if (mJson.find(Constants::ASSET_ATTR_SPRITE_WIDTH) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_SPRITE_WIDTH] = width;
            }
            width = mJson[Constants::ASSET_ATTR_SPRITE_WIDTH];
            return width;
        } dreamElseLockFailed
    }

    void SpriteDefinition::setHeight(int height)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::ASSET_ATTR_SPRITE_HEIGHT] = height;
        } dreamElseLockFailed
    }

    int SpriteDefinition::getHeight()
    {
        if(dreamTryLock()) {
            dreamLock();

            int height = 1;
            if (mJson.find(Constants::ASSET_ATTR_SPRITE_HEIGHT) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_SPRITE_HEIGHT] = height;
            }
            height = mJson[Constants::ASSET_ATTR_SPRITE_HEIGHT];
            return height;
        } dreamElseLockFailed
    }

    void SpriteDefinition::setTexture(UuidType id)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::ASSET_ATTR_SPRITE_TEXTURE] = id;
        } dreamElseLockFailed
    }

    UuidType SpriteDefinition::getTexture()
    {
        if(dreamTryLock()) {
            dreamLock();

            UuidType id;
            if (mJson.find(Constants::ASSET_ATTR_SPRITE_TEXTURE) == mJson.end())
            {
                mJson[Constants::ASSET_ATTR_SPRITE_TEXTURE] = id;
            }
            id = mJson[Constants::ASSET_ATTR_SPRITE_TEXTURE];
            return id;
        } dreamElseLockFailed
    }
}


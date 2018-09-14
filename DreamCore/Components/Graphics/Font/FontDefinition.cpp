/*
 * FontDefinition.cpp
 *
 * Created: 11 2017 by Ashley
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
#include "FontDefinition.h"

namespace Dream
{
    FontDefinition::FontDefinition
    (const shared_ptr<ProjectDefinition>& pd, json js)
        :IAssetDefinition(pd,js)
    {
        makeColourObject();
    }

    void
    FontDefinition::setColourRed
    (float value)
    {
       makeColourObject();
       mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::RED] = value;
    }

    float
    FontDefinition::getColourRed
    ()
    {
        makeColourObject();
        if (mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::RED].is_null())
        {
            mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::RED] = 0;
        }

        return mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::RED];
    }

    void
    FontDefinition::setColourGreen
    (float value)
    {
        makeColourObject();
        mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::GREEN] = value;
    }

    float
    FontDefinition::getColourGreen
    ()
    {
        makeColourObject();
        if (mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::GREEN].is_null())
        {
            mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::GREEN] = 0;
        }

        return mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::GREEN];

    }

    void
    FontDefinition::setColourBlue
    (float value)
    {
        makeColourObject();
       mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::BLUE] = value;
    }

    float
    FontDefinition::getColourBlue
    ()
    {
        makeColourObject();
        if (mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::BLUE].is_null())
        {
            mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::BLUE] = 0;
        }

        return mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::BLUE];

    }

    void
    FontDefinition::setColourAlpha
    (float value)
    {
        makeColourObject();
       mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::ALPHA] = value;
    }

    float
    FontDefinition::getColourAlpha
    ()
    {
        makeColourObject();
        return mJson[Constants::ASSET_ATTR_LIGHT_COLOR][Constants::ALPHA];
    }

    void
    FontDefinition::makeColourObject
    ()
    {
        if (mJson[Constants::ASSET_ATTR_LIGHT_COLOR].is_null())
        {
            mJson[Constants::ASSET_ATTR_LIGHT_COLOR] = json::object();
        }
    }

    void
    FontDefinition::setSize
    (float size)
    {
       mJson[Constants::ASSET_ATTR_SIZE] = size;
    }

    float
    FontDefinition::getSize
    ()
    {
       if (mJson[Constants::ASSET_ATTR_SIZE].is_null())
       {
           mJson[Constants::ASSET_ATTR_SIZE] = 0;
       }

       return mJson[Constants::ASSET_ATTR_SIZE];
    }
}

/*
 * AudioDefinition.cpp
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

#include "AudioDefinition.h"

namespace Dream
{
    AudioDefinition::AudioDefinition
    (weak_ptr<ProjectDefinition> pd, json js)
        : IAssetDefinition(pd,js)
    {

    }

    bool
    AudioDefinition::isFormatWav
    ()
    {
        return getFormat().compare(Constants::ASSET_FORMAT_AUDIO_WAV) == 0;
    }

    bool
    AudioDefinition::isFormatOgg
    ()
    {
        return getFormat().compare(Constants::ASSET_FORMAT_AUDIO_OGG) == 0;
    }

    void
    AudioDefinition::setLoop
    (bool loop)
    {
        mJson[Constants::ASSET_ATTR_LOOP] = loop;
    }

    bool
    AudioDefinition::getLoop
    ()
    {
        if (mJson[Constants::ASSET_ATTR_LOOP].is_null())
        {
            mJson[Constants::ASSET_ATTR_LOOP] = false;
        }
        return mJson[Constants::ASSET_ATTR_LOOP];
    }


}

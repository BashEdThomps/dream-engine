/*
 * IAudioInstance.cpp
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
#include "IAudioInstance.h"

namespace Dream
{
    IAudioInstance::IAudioInstance(AssetDefinition *def, Transform3D *transform)
        : IAssetInstance(def,transform)
    {

    }

    IAudioInstance::~IAudioInstance() {}

    void IAudioInstance::setLooping(bool looping)
    {
        mLooping = looping;
    }

    bool IAudioInstance::isLooping()
    {
        return mLooping;
    }

    AudioStatus IAudioInstance::getStatus()
    {
        return AudioStatus::UNKNOWN;
    }

} // End of Dream

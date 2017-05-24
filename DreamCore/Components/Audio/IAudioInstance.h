/*
 * IAudioInstance.h
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
#ifndef IAUDIOINSTANCE_H
#define IAUDIOINSTANCE_H

#include "../../IAssetInstance.h"
#include "AudioStatus.h"

namespace Dream
{
    class IAudioInstance : public IAssetInstance
    {
    public:
        IAudioInstance(AssetDefinition*, Transform3D*);
        virtual ~IAudioInstance();
        virtual bool play() = 0;
        virtual bool pause() = 0;
        virtual bool stop() = 0;
        AudioStatus getStatus();
        void setLooping(bool);
        bool isLooping();
    protected:
        bool mLooping;
    };
}

#endif // IAUDIOINSTANCE_H

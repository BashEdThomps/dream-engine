/*
 * QTDreamAudioInstance.h
 *
 * Created: 22 2017 by Ashley
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
#ifndef QTDREAMAUDIOINSTANCE_H
#define QTDREAMAUDIOINSTANCE_H

#include <DreamCore.h>

class QTDreamAudioInstance : public Dream::IAudioInstance
{
public:
    QTDreamAudioInstance(Dream::AssetDefinition* definition, Dream::Transform3D* transform);
    ~QTDreamAudioInstance();
    bool play();
    bool pause();
    bool stop();

    bool load(string);
    void loadExtraAttributes(nlohmann::json);

};

#endif // QTDREAMAUDIOINSTANCE_H

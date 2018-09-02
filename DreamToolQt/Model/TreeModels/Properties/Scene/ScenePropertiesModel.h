/*
 * ScenePropertiesModel.h
 *
 * Created: 17 2017 by Ashley
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

#pragma once

#include "../AbstractPropertiesModel.h"
#include <DreamCore.h>

using Dream::SceneDefinition;

class ScenePropertiesModel : public AbstractPropertiesModel
{
    Q_OBJECT
public:

    ScenePropertiesModel(SceneDefinition *scene, QTreeView* parent = nullptr);
    ~ScenePropertiesModel() override;

    void createRoot() override;
    void createProperties() override;
    void createDelegateConnections() override;

    void createNameProperties();
    void createNotesProperties();
    void createCameraProperties();
    void createRenderingProperties();
    void createPhysicsProperties();

public slots:
    void onButton_CaptureCameraTranslation();
    void onButton_CaptureCameraLookAt();
    void onButton_ChooseClearColour();
    void onButton_ChooseAmbientColour();

signals:
    void notifyButton_CaptureCameraTranslation(SceneDefinition*);
    void notifyButton_CaptureCameraLookAt(SceneDefinition*);
    void notifyButton_ChooseClearColour(SceneDefinition*);
    void notifyButton_ChooseAmbientColour(SceneDefinition*);

private:
    SceneDefinition *mSceneDefinitionHandle;
};


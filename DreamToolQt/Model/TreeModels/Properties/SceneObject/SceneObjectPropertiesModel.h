/*
 * SceneObjectPropertiesModel.h
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

namespace Dream
{
    class SceneObjectDefinition;
    class IAssetDefinition;
}

using Dream::SceneObjectDefinition;
using Dream::IAssetDefinition;

class SceneObjectPropertiesModel : public AbstractPropertiesModel
{
    Q_OBJECT
public:
    SceneObjectPropertiesModel(SceneObjectDefinition *sceneObject,QTreeView* parent = nullptr);
    ~SceneObjectPropertiesModel() override;

    void createRoot() override;
    void createProperties() override;
    void createDelegateConnections() override;

    void createNameProperty();
    void createTranslationProperty();
    void createRotationProperty();
    void createScaleProperty();
    void createTransformTypeProperty();
    void createHasFocusProperty();
    void createFollowsCameraProperty();
    void createAssetInstancesProperty();
    void createChildrenProperty();


public slots:
    void onButton_CaptureTranslation();
    void onButton_CaptureRotation();
    void onButton_CaptureScale();
    void onButton_RemoveAsset(IAssetDefinition*);
    void onButton_RemoveChild(SceneObjectDefinition*);

signals:
    void notifyButton_CaptureTranslation(SceneObjectDefinition*);
    void notifyButton_CaptureRotation(SceneObjectDefinition*);
    void notifyButton_CaptureScale(SceneObjectDefinition*);

    void notifyButton_RemoveAsset(SceneObjectDefinition*, IAssetDefinition*);
    void notifyButton_RemoveChild(SceneObjectDefinition*, SceneObjectDefinition*);

private:
    SceneObjectDefinition *mSceneObjectDefinitionHandle;
};
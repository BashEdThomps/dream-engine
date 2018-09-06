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
    class IDefinition;
}

using std::shared_ptr;
using Dream::SceneObjectDefinition;
using Dream::IAssetDefinition;
using Dream::IDefinition;

class SceneObjectPropertiesModel : public AbstractPropertiesModel
{
    Q_OBJECT
public:
    SceneObjectPropertiesModel(shared_ptr<SceneObjectDefinition> sceneObject,QTreeView* parent = nullptr);
    ~SceneObjectPropertiesModel() override;

    void createRoot() override;
    void createProperties() override;
    void createDelegateConnections() override;

    void createNameProperty();
    void createTranslationProperty();
    void createOrientationProperty();
    void createScaleProperty();
    void createTransformTypeProperty();
    void createHasFocusProperty();
    void createAlwaysDrawProperty();
    void createFollowsCameraProperty();
    void createAssetInstancesProperty();
    void createChildrenProperty();
    void createStaticProperty();


public slots:
    void onButton_CaptureTranslation();
    void onButton_CaptureOrientation();
    void onButton_CaptureScale();
    void onButton_RemoveAsset(shared_ptr<IDefinition>);
    void onButton_RemoveChild(shared_ptr<IDefinition>);

signals:
    void notifyButton_CaptureTranslation(shared_ptr<SceneObjectDefinition>);
    void notifyButton_CaptureOrientation(shared_ptr<SceneObjectDefinition>);
    void notifyButton_CaptureScale(shared_ptr<SceneObjectDefinition>);

    void notifyButton_RemoveAsset(shared_ptr<SceneObjectDefinition>, shared_ptr<IDefinition>);
    void notifyButton_RemoveChild(shared_ptr<SceneObjectDefinition>, shared_ptr<IDefinition>);

private:
    shared_ptr<SceneObjectDefinition> mSceneObjectDefinitionHandle;
};

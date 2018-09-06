/*
 * DreamModel.h
 *
 * Created: 14 2017 by Ashley
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

#include <memory>

#include <DreamCore.h>

#include "../View/QOpenGLWindowComponent.h"

#include <QObject>

namespace Qt
{
    class QTimer;
}

using std::string;
using std::vector;

using Dream::IAssetDefinition;
using Dream::SceneDefinition;
using Dream::Project;
using Dream::IDefinition;

class DreamProjectModel : public QObject
{
    Q_OBJECT
public:
    DreamProjectModel
    (
        QObject *parent = nullptr,
        shared_ptr<QOpenGLWindowComponent> windowComponent = nullptr
    );
    ~DreamProjectModel() override;

    bool loadProject(QString path);
    shared_ptr<Project> getProject();

    vector<shared_ptr<SceneDefinition>> getScenes();
    vector<shared_ptr<IAssetDefinition>> getAssetDefinitions();

    void setProjectName(string name);
    void setProjectAuthor(string author);
    void setProjectDescription(string desc);
    void setProjectStartupSceneByUuid(string scene);
    void setProjectWindowWidth(int width);
    void setProjectWindowHeight(int height);
    void setDebug(bool enabled);
    void setPhysicsDebug(bool enabled);
    void closeProject();

    bool startSceneRuntimeFromDefinition(shared_ptr<SceneDefinition> definition);
    shared_ptr<SceneRuntime> stopActiveSceneRuntime();
    void setupHeartbeatTimer();

    shared_ptr<IAssetDefinition> getAssetDefinitionByUuid(string uuid);
    shared_ptr<SceneDefinition> getSceneDefinitionByUuid(string uuid);

    shared_ptr<SceneDefinition> getSelectedSceneDefinition();
    void setSelectedSceneDefinition(shared_ptr<SceneDefinition> selectedScene);

    shared_ptr<IDefinition> createNewAssetDefinition(AssetType type);

signals:
    void notifySelectedSceneChanged(shared_ptr<SceneDefinition> scene);

private:
    // Owned Objects
    shared_ptr<Project> mProject;
    shared_ptr<QTimer>  mHeartbeatTimer;

    // Handles
    shared_ptr<QOpenGLWindowComponent> mWindowComponent;
    shared_ptr<SceneDefinition> mSelectedScene;
};

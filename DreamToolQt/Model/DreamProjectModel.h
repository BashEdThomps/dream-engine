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

using Dream::AssetDefinition;
using Dream::SceneDefinition;
using Dream::Project;

class DreamProjectModel : public QObject
{
    Q_OBJECT
public:
    DreamProjectModel
    (
        QObject *parent = nullptr,
        QOpenGLWindowComponent *windowComponent = nullptr
    );
    ~DreamProjectModel();
    bool loadProject(QString path);
    Project* getProject();
    vector<SceneDefinition*> getScenes();
    vector<AssetDefinition*> getAssetDefinitions();
    void setProjectName(string name);
    void setProjectAuthor(string author);
    void setProjectDescription(string desc);
    void setProjectStartupSceneByUuid(string scene);
    void setProjectWindowWidth(int width);
    void setProjectWindowHeight(int height);
    void setDebug(bool enabled);
    void closeProject();

    bool startSceneRuntimeFromDefinition(SceneDefinition* definitionHandle);
    SceneRuntime* stopActiveSceneRuntime();
    void setupHeartbeatTimer();

    AssetDefinition* getAssetDefinitionHandleByUuid(string uuid);
    SceneDefinition* getSceneDefinitionHandleByUuid(string uuid);

    SceneDefinition* getSelectedSceneDefinitionHandle();
    void setSelectedSceneDefinitionHandle(SceneDefinition* selectedScene);

signals:
    void notifySelectedSceneChanged(SceneDefinition* scene);

private:
    // Owned Objects
    unique_ptr<Project> mProject;
    unique_ptr<QTimer>  mHeartbeatTimer;

    // Handles
    QOpenGLWindowComponent *mWindowComponentHandle;
    SceneDefinition* mSelectedSceneHandle;
};

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
#ifndef DREAMMODEL_H
#define DREAMMODEL_H

#include <DreamCore.h>
#include <QObject>
#include <QTimer>
#include <memory>

#include "QOpenGLWindowComponent.h"

using namespace std;
using namespace Dream;

class DreamModel : public QObject
{
    Q_OBJECT
public:
    explicit DreamModel(QObject *parent = 0,
                        QOpenGLWindowComponent *windowComponent = 0);
    ~DreamModel();
    bool loadProject(QString path);
    Project* getProject();
    vector<Scene*> getScenes();
    vector<AssetDefinition*> getAssetDefinitions();
    void setProjectName(string name);
    void setProjectAuthor(string author);
    void setProjectDescription(string desc);
    void setProjectStartupSceneByUuid(string scene);
    void setProjectStartupSceneByName(string scene);
    void setProjectWindowWidth(int width);
    void setProjectWindowHeight(int height);
    void setDebug(bool enabled);

    bool startScene();
    Scene* stopActiveScene();
    void setupHeartbeatTimer();

    AssetDefinition *getAssetDefinitionByUuid(std::string uuid);
    Scene *getSceneByUuid(std::string uuid);

    Scene *getSelectedScene();
    void setSelectedScene(Scene* selectedScene);
signals:
    void notifySelectedSceneChanged(Scene* scene);


private:
    unique_ptr<Project>mProject;
    Scene* mSelectedScene;
    QOpenGLWindowComponent *mWindowComponent;
    QTimer* mHeartbeatTimer;
};

#endif // DREAMMODEL_H

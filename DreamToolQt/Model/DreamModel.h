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

#include <QObject>
#include <DreamCore.h>

#include "QTDreamAudioComponent.h"
#include "QTDreamWindowComponent.h"

class DreamModel : public QObject
{
    Q_OBJECT
public:
    explicit DreamModel(QObject *parent = 0,
                        Dream::IAudioComponent *audioComponent = 0,
                        Dream::IWindowComponent *windowComponent = 0);
    ~DreamModel();
    bool loadProject(QString path);
    Dream::Project* getProject();
    vector<Dream::Scene*>* getScenes();
    vector<Dream::AssetDefinition*>* getAssetDefinitions();
    void setProjectName(string name);
    void setProjectAuthor(string author);
    void setProjectDescription(string desc);
    void setProjectStartupSceneByUuid(string scene);
    void setProjectStartupSceneByName(string scene);
    void setProjectWindowWidth(int width);
    void setProjectWindowHeight(int height);
    bool reloadProject(Dream::Scene* scene);
    Dream::AssetDefinition *getAssetDefinitionByUuid(std::string uuid);
    Dream::Scene *getSceneByUuid(std::string uuid);
    Dream::SceneObject* getSceneObjectByUuid(std::string uuid);

    Dream::Scene *getSelectedScene();
    void setSelectedScene(Dream::Scene* selectedScene);

    int heartbeatDream();
signals:

public slots:

private:
    Dream::DreamEngine* mDreamEngine;
    Dream::Scene* mSelectedScene;
};

#endif // DREAMMODEL_H

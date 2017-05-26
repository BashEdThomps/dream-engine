/*
 * DreamModel.cpp
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
#include "DreamModel.h"
#include <QDebug>

DreamModel::DreamModel(
        QObject *parent,
        QTDreamAudioComponent *audioComponent,
        QTDreamWindowComponent *windowComponent)
    : QObject(parent)
{
    mDreamEngine = new Dream::DreamEngine(audioComponent,windowComponent);
    mAudioComponent = audioComponent;
    mWindowComponent = windowComponent;
    mSelectedScene = nullptr;
    mHeartbeatTimer = nullptr;
}

DreamModel::~DreamModel()
{
}

Dream::Project* DreamModel::getProject()
{
    if (mDreamEngine)
    {
        return mDreamEngine->getProject();
    }
    return nullptr;
}

vector<Dream::Scene*>* DreamModel::getScenes()
{
    if (mDreamEngine)
    {
        return nullptr;
    }
    return nullptr;
}

vector<Dream::AssetDefinition*>* DreamModel::getAssetDefinitions()
{
    if (mDreamEngine)
    {
        return nullptr;
    }
    return nullptr;
}


bool DreamModel::loadProject(QString path)
{
    return mDreamEngine->loadFromDirectory(path.toStdString());
}

void DreamModel::setProjectName(string name)
{
    mDreamEngine->getProject()->setName(name);
}

void DreamModel::setProjectAuthor(string author)
{
    mDreamEngine->getProject()->setAuthor(author);
}

void DreamModel::setProjectDescription(string desc)
{
    mDreamEngine->getProject()->setDescription(desc);
}

void DreamModel::setProjectStartupSceneByUuid(string scene)
{
    mDreamEngine->getProject()->setStartupSceneUuid(scene);
}

void DreamModel::setProjectStartupSceneByName(string scene)
{
    mDreamEngine->getProject()->setStartupSceneName(scene);
}

void DreamModel::setProjectWindowWidth(int width)
{
    mDreamEngine->getProject()->setWindowWidth(width);
}

void DreamModel::setProjectWindowHeight(int height)
{
    mDreamEngine->getProject()->setWindowHeight(height);
}

Dream::AssetDefinition* DreamModel::getAssetDefinitionByUuid(std::string uuid)
{
    return mDreamEngine->getProject()->getAssetDefinitionByUuid(uuid);
}

Dream::Scene* DreamModel::getSceneByUuid(std::string uuid)
{
    return mDreamEngine->getProject()->getSceneByUuid(uuid);
}

bool DreamModel::startScene(Dream::Scene* scene)
{
    qDebug() << "DreamModel: *** Start Scene ***";
    bool initResult = mDreamEngine->initComponents();
    if (!initResult)
    {
        qDebug() << "DreamModel: Error initialising dream components";
        return false;
    }

    bool loadResult = mDreamEngine->loadScene(scene);
    if (!loadResult)
    {
        qDebug() << "DreamModel: Error initialising dream Project";
        return false;
    }

    if (mHeartbeatTimer)
    {
        disconnect(mHeartbeatTimer, SIGNAL(timeout()), this, SLOT(onDreamHeartbeat()));
        delete mHeartbeatTimer;
    }

    mWindowComponent->setDreamEngine(mDreamEngine);
    mHeartbeatTimer = new QTimer(this);
    connect(mHeartbeatTimer, SIGNAL(timeout()), mWindowComponent, SLOT(update()),Qt::DirectConnection);
    mHeartbeatTimer->start(16);

    return true;
}

Dream::Scene *DreamModel::getSelectedScene()
{
    return mSelectedScene;
}

void DreamModel::setSelectedScene(Dream::Scene* selectedScene)
{
    mSelectedScene = selectedScene;
}

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

DreamModel::DreamModel
(QObject *parent,QOpenGLWindowComponent *windowComponent)
    : QObject(parent)
{
    mWindowComponent = windowComponent;
    mProject.reset(new Project(windowComponent));
    mProject->initRuntime();
    mSelectedScene = nullptr;
    mHeartbeatTimer = nullptr;
    setupHeartbeatTimer();
}

void
DreamModel::setupHeartbeatTimer
()
{
    if (!mHeartbeatTimer)
    {
        mHeartbeatTimer = new QTimer(this);
        connect(mHeartbeatTimer, SIGNAL(timeout()), mWindowComponent, SLOT(update()),Qt::DirectConnection);
        mHeartbeatTimer->start(16);
    }
}

DreamModel::~DreamModel
()
{
}

Project*
DreamModel::getProject
()
{
    return mProject.get();
}

vector<Scene*>
DreamModel::getScenes
()
{
    return mProject->getSceneList();
}

vector<AssetDefinition*>
DreamModel::getAssetDefinitions
()
{
    return mProject->getAssetDefinitions();
}

bool
DreamModel::loadProject
(QString path)
{
    return mProject->loadFromDirectory(path.toStdString());
}

void
DreamModel::setProjectName
(string name)
{
    mProject->setName(name);
}

void
DreamModel::setProjectAuthor
(string author)
{
    mProject->setAuthor(author);
}

void
DreamModel::setProjectDescription
(string desc)
{
    mProject->setDescription(desc);
}

void
DreamModel::setProjectStartupSceneByUuid
(string scene)
{
    mProject->setStartupSceneUuid(scene);
}

void
DreamModel::setProjectStartupSceneByName
(string scene)
{
    mProject->setStartupSceneName(scene);
}

void
DreamModel::setProjectWindowWidth
(int width)
{
    mProject->setWindowWidth(width);
}

void
DreamModel::setProjectWindowHeight
(int height)
{
    mProject->setWindowHeight(height);
}

AssetDefinition*
DreamModel::getAssetDefinitionByUuid
(std::string uuid)
{
    return mProject->getAssetDefinitionByUuid(uuid);
}

Scene*
DreamModel::getSceneByUuid
(std::string uuid)
{
    return getProject()->getSceneByUuid(uuid);
}

bool
DreamModel::startScene
()
{
    if (!mSelectedScene)
    {
        qDebug() << "DreamModel: No scene selected";
        return false;
    }

    qDebug() << "DreamModel: *** Start Scene ***";
    mProject->initRuntime();

    mProject->setActiveScene(mSelectedScene);
    bool loadResult = mProject->loadActiveScene();

    if (!loadResult)
    {
        qDebug() << "DreamModel: Error initialising dream Project";
        return false;
    }



    mWindowComponent->setProject(mProject.get());
    return true;
}

Scene*
DreamModel::getSelectedScene
()
{
    return mSelectedScene;
}

void
DreamModel::setSelectedScene
(Scene* selectedScene)
{
    mSelectedScene = selectedScene;
    //emit notifySelectedSceneChanged(selectedScene);
}

Scene*
DreamModel::stopActiveScene
()
{
    Scene* activeScene = mProject->getActiveScene();
    if (activeScene)
    {
        activeScene->setState(DONE);
        mProject->cleanUpActiveScene();
        mProject->setActiveScene(nullptr);

    }
    return activeScene;
}

/*
 * DreamProjectModel.cpp
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
#include "DreamProjectModel.h"
#include <QDebug>

DreamProjectModel::DreamProjectModel
(QObject *parent,QOpenGLWindowComponent *windowComponent)
    : QObject(parent),
      mWindowComponentHandle(windowComponent),
      mSelectedSceneHandle(nullptr)
{
    setupHeartbeatTimer();
    mProject.reset(new Project(mWindowComponentHandle));
    mProject->initRuntime();
}

void
DreamProjectModel::setupHeartbeatTimer
()
{
    if (!mHeartbeatTimer)
    {
        mHeartbeatTimer.reset(new QTimer(this));
        connect(mHeartbeatTimer.get(), SIGNAL(timeout()), mWindowComponentHandle, SLOT(update()),Qt::DirectConnection);
        mHeartbeatTimer->start(16);
    }
}

DreamProjectModel::~DreamProjectModel
()
{
}

Project*
DreamProjectModel::getProject
()
{
    return mProject.get();
}

vector<Scene*>
DreamProjectModel::getScenes
()
{
    return mProject->getSceneList();
}

vector<AssetDefinition*>
DreamProjectModel::getAssetDefinitions
()
{
    return mProject->getAssetDefinitions();
}

bool
DreamProjectModel::loadProject
(QString path)
{
    mProject.reset(new Project(mWindowComponentHandle));
    mProject->initRuntime();
    return mProject->loadFromDirectory(path.toStdString());
}

void
DreamProjectModel::setProjectName
(string name)
{
    mProject->setName(name);
}

void
DreamProjectModel::setProjectAuthor
(string author)
{
    mProject->setAuthor(author);
}

void
DreamProjectModel::setProjectDescription
(string desc)
{
    mProject->setDescription(desc);
}

void
DreamProjectModel::setProjectStartupSceneByUuid
(string scene)
{
    mProject->setStartupSceneUuid(scene);
}

void
DreamProjectModel::setProjectWindowWidth
(int width)
{
    mProject->setWindowWidth(width);
}

void
DreamProjectModel::setProjectWindowHeight
(int height)
{
    mProject->setWindowHeight(height);
}

AssetDefinition*
DreamProjectModel::getAssetDefinitionByUuid
(std::string uuid)
{
    return mProject->getAssetDefinitionByUuid(uuid);
}

Scene*
DreamProjectModel::getSceneByUuid
(std::string uuid)
{
    return getProject()->getSceneByUuid(uuid);
}

bool
DreamProjectModel::startScene
()
{
    if (!mSelectedSceneHandle)
    {
        qDebug() << "DreamModel: No scene selected";
        return false;
    }

    qDebug() << "DreamModel: *** Start Scene ***";
    mProject->setActiveScene(mSelectedSceneHandle);
    bool loadResult = mProject->loadActiveScene();

    if (!loadResult)
    {
        qDebug() << "DreamModel: Error initialising dream Project";
        return false;
    }

    mWindowComponentHandle->setProject(mProject.get());
    return true;
}

Scene*
DreamProjectModel::getSelectedScene
()
{
    return mSelectedSceneHandle;
}

void
DreamProjectModel::setSelectedScene
(Scene* selectedScene)
{
    mSelectedSceneHandle = selectedScene;
}

Scene*
DreamProjectModel::stopActiveScene
()
{
    Scene* activeScene = mProject->getActiveScene();
    if (activeScene)
    {
        activeScene->getRuntime()->setState(DONE);
        mProject->cleanUpActiveScene();
        mProject->setActiveScene(nullptr);

    }
    return activeScene;
}

void
DreamProjectModel::setDebug
(bool enabled)
{
    Constants::dreamSetVerbose(enabled);
    Constants::dreamSetDebug(enabled);
}

void
DreamProjectModel::closeProject
()
{
    stopActiveScene();
    mProject.reset(nullptr);
    mHeartbeatTimer.reset(nullptr);
}

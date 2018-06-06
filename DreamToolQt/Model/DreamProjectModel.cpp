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

#include <QTimer>
#include <QDebug>

DreamProjectModel::DreamProjectModel
(QObject *parent,QOpenGLWindowComponent *windowComponent)
    : QObject(parent),
      mWindowComponentHandle(windowComponent),
      mSelectedSceneHandle(nullptr)
{
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

vector<SceneDefinition*>
DreamProjectModel::getScenes
()
{
    return mProject->getProjectDefinitionHandle()->getSceneDefinitionsHandleList();
}

vector<IAssetDefinition*>
DreamProjectModel::getAssetDefinitions
()
{
    return mProject->getProjectDefinitionHandle()->getAssetDefinitionsHandleList();
}

bool
DreamProjectModel::loadProject
(QString path)
{
    mProject.reset(new Project(mWindowComponentHandle));
    return mProject->openFromDirectory(path.toStdString());
}

void
DreamProjectModel::setProjectName
(string name)
{
    mProject->getProjectDefinitionHandle()->setName(name);
}

void
DreamProjectModel::setProjectAuthor
(string author)
{
    mProject->getProjectDefinitionHandle()->setAuthor(author);
}

void
DreamProjectModel::setProjectDescription
(string desc)
{
    mProject->getProjectDefinitionHandle()->setDescription(desc);
}

void
DreamProjectModel::setProjectStartupSceneByUuid
(string scene)
{
    mProject->getProjectDefinitionHandle()->setStartupSceneUuid(scene);
}

void
DreamProjectModel::setProjectWindowWidth
(int width)
{
    mProject->getProjectDefinitionHandle()->setWindowWidth(width);
}

void
DreamProjectModel::setProjectWindowHeight
(int height)
{
    mProject->getProjectDefinitionHandle()->setWindowHeight(height);
}

IAssetDefinition*
DreamProjectModel::getAssetDefinitionHandleByUuid
(std::string uuid)
{
    return mProject->getProjectDefinitionHandle()->getAssetDefinitionHandleByUuid(uuid);
}

SceneDefinition*
DreamProjectModel::getSceneDefinitionHandleByUuid
(string uuid)
{
    return getProject()->getProjectDefinitionHandle()->getSceneDefinitionHandleByUuid(uuid);
}

bool
DreamProjectModel::startSceneRuntimeFromDefinition
(SceneDefinition* definitionHandle)
{
    if (!definitionHandle)
    {
        qDebug() << "DreamModel: No SceneDefinition selected";
        return false;
    }
    qDebug() << endl << "          ===== DreamModel - Start Scene =====" << endl;
    ProjectRuntime* prHandle = mProject->createProjectRuntime();
    SceneRuntime* srHandle = prHandle->constructActiveSceneRuntime(definitionHandle);
    mWindowComponentHandle->setProjectRuntimeHandle(mProject->getProjectRuntimeHandle());
    setupHeartbeatTimer();
    return srHandle != nullptr;
}

SceneDefinition*
DreamProjectModel::getSelectedSceneDefinitionHandle
()
{
    return mSelectedSceneHandle;
}

void
DreamProjectModel::setSelectedSceneDefinitionHandle
(SceneDefinition* selectedScene)
{
    mSelectedSceneHandle = selectedScene;
}

SceneRuntime*
DreamProjectModel::stopActiveSceneRuntime
()
{
    mWindowComponentHandle->setProjectRuntimeHandle(nullptr);

    if (mProject)
    {
        ProjectRuntime* prHandle = mProject->getProjectRuntimeHandle();
        if (prHandle)
        {
            SceneRuntime *srHandle = nullptr;
            srHandle = prHandle->getActiveSceneRuntimeHandle();

            if (srHandle)
            {
                srHandle->setState(SCENE_STATE_STOPPED);
                prHandle->resetActiveSceneRuntime();
            }
        }
        mProject->resetProjectRuntime();
    }
    return nullptr;
}

void
DreamProjectModel::setDebug
(bool enabled)
{
    Constants::dreamSetVerbose(enabled);
    Constants::dreamSetDebug(enabled);
}

void DreamProjectModel::setPhysicsDebug(bool enabled)
{
    if (mProject)
    {
        ProjectRuntime* prHandle = mProject->getProjectRuntimeHandle();
        if (prHandle)
        {
            prHandle->getPhysicsComponentHandle()->setDebug(enabled);
        }
    }
}

void
DreamProjectModel::closeProject
()
{
    stopActiveSceneRuntime();
    mProject.reset(nullptr);
    mHeartbeatTimer.reset(nullptr);
}

IDefinition*
DreamProjectModel::createNewAssetDefinition
(AssetType type)
{
    if (mProject)
    {
        ProjectDefinition* pdHandle = mProject->getProjectDefinitionHandle();
        if (pdHandle)
        {
            IAssetDefinition *adHandle = pdHandle->createNewAssetDefinition(type);
            if (adHandle)
            {
                return adHandle;
            }
        }
    }
    return nullptr;
}

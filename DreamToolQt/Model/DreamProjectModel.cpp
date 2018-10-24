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
(QObject *parent,QOpenGLWindowComponent* windowComponent)
    : QObject(parent),
      mProject(nullptr),
      mHeartbeatTimer(nullptr),
      mWindowComponent(windowComponent),
      mSelectedScene(nullptr),
      mScriptingEnabled(true)
{
    auto log = spdlog::get("DreamProjectModel");
    if (log == nullptr)
    {
        log = spdlog::stdout_color_mt("DreamProjectModel");
    }
}

void
DreamProjectModel::setupHeartbeatTimer
()
{
    if (mHeartbeatTimer == nullptr)
    {
        mHeartbeatTimer = new QTimer(this);
        connect(
            mHeartbeatTimer,
            SIGNAL(timeout()),
            mWindowComponent,
            SLOT(update()),Qt::DirectConnection
        );
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
    return mProject;
}

vector<SceneDefinition*>
DreamProjectModel::getScenes
()
{
    return mProject->getProjectDefinition()->getSceneDefinitionsList();
}

vector<IAssetDefinition* >
DreamProjectModel::getAssetDefinitions
()
{
    return mProject->getProjectDefinition()->getAssetDefinitionsList();
}

bool
DreamProjectModel::loadProject
(QString path)
{
    mProject = new Project(mWindowComponent);
    return mProject->openFromDirectory(path.toStdString());
}

void
DreamProjectModel::setProjectName
(string name)
{
    mProject->getProjectDefinition()->setName(name);
}

void
DreamProjectModel::setProjectAuthor
(string author)
{
    mProject->getProjectDefinition()->setAuthor(author);
}

void
DreamProjectModel::setProjectDescription
(string desc)
{
    mProject->getProjectDefinition()->setDescription(desc);
}

void
DreamProjectModel::setProjectStartupSceneByUuid
(string scene)
{
    mProject->getProjectDefinition()->setStartupSceneUuid(scene);
}

void
DreamProjectModel::setProjectWindowWidth
(int width)
{
    mProject->getProjectDefinition()->setWindowWidth(width);
}

void
DreamProjectModel::setProjectWindowHeight
(int height)
{
    mProject->getProjectDefinition()->setWindowHeight(height);
}

IAssetDefinition*
DreamProjectModel::getAssetDefinitionByUuid
(std::string uuid)
{
    return mProject
        ->getProjectDefinition()
        ->getAssetDefinitionByUuid(uuid);
}

SceneDefinition*
DreamProjectModel::getSceneDefinitionByUuid
(string uuid)
{
    return getProject()->getProjectDefinition()->getSceneDefinitionByUuid(uuid);
}

bool
DreamProjectModel::startSceneRuntimeFromDefinition
(SceneDefinition* definition)
{
    auto log = spdlog::get("DreamProjectModel");
    if (!definition)
    {
        log->debug("No SceneDefinition selected");
        return false;
    }
    log->debug("\n===== DreamModel - Start Scene =====");

    ProjectRuntime* prHandle = mProject->createProjectRuntime();
    if (prHandle != nullptr)
    {
        prHandle->setScriptingEnabled(mScriptingEnabled);
        SceneRuntime* srHandle = prHandle->constructActiveSceneRuntime(definition);
        if (srHandle != nullptr)
        {
            mWindowComponent
                ->setProjectRuntime(mProject->getProjectRuntime());
            setupHeartbeatTimer();
            return srHandle != nullptr;
        }
    }
    return false;
}

SceneDefinition*
DreamProjectModel::getSelectedSceneDefinition
()
{
    return mSelectedScene;
}

void
DreamProjectModel::setSelectedSceneDefinition
(SceneDefinition*  selectedScene)
{
    mSelectedScene = selectedScene;
}

void
DreamProjectModel::stopActiveSceneRuntime
()
{
    if (mProject != nullptr)
    {
        ProjectRuntime* prHandle = mProject->getProjectRuntime();
        if (prHandle)
        {
            SceneRuntime* srHandle = prHandle->getActiveSceneRuntime();
            if (srHandle)
            {
                srHandle->setState(SCENE_STATE_STOPPED);
            }
        }

        if (mHeartbeatTimer != nullptr)
        {
            mHeartbeatTimer->stop();
            delete mHeartbeatTimer;
            mHeartbeatTimer = nullptr;
        }

        mProject->resetProjectRuntime();
    }
}

void
DreamProjectModel::setDebug
(bool)
{
}

void DreamProjectModel::setPhysicsDebug(bool enabled)
{
    if (mProject)
    {
        ProjectRuntime* prHandle = mProject->getProjectRuntime();
        if (prHandle)
        {
            auto physics = prHandle->getPhysicsComponent();
            if (physics != nullptr)
            {
                physics->setDebug(enabled);
            }
        }
    }
}

void
DreamProjectModel::closeProject
()
{
    stopActiveSceneRuntime();
    mProject = nullptr;
}

IDefinition*
DreamProjectModel::createNewAssetDefinition
(AssetType type)
{
    if (mProject)
    {
        ProjectDefinition* pdHandle = mProject->getProjectDefinition();
        if (pdHandle)
        {
            IAssetDefinition* adHandle = pdHandle->createNewAssetDefinition(type);
            if (adHandle)
            {
                return adHandle;
            }
        }
    }
    return nullptr;
}

void
DreamProjectModel::setScriptingEnabled(bool enabled)
{
   mScriptingEnabled = enabled;
   if (mProject)
   {
       auto prHandle = mProject->getProjectRuntime();
       if (prHandle)
       {
           prHandle->setScriptingEnabled(enabled);
       }
   }
}

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
#include <spdlog/spdlog.h>

DreamProjectModel::DreamProjectModel
(QObject *parent,shared_ptr<QOpenGLWindowComponent> windowComponent)
    : QObject(parent),
      mWindowComponent(windowComponent)
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
    if (!mHeartbeatTimer)
    {
        mHeartbeatTimer.reset(new QTimer(this));
        connect(mHeartbeatTimer.get(), SIGNAL(timeout()), mWindowComponent.get(), SLOT(update()),Qt::DirectConnection);
        mHeartbeatTimer->start(16);
    }
}

DreamProjectModel::~DreamProjectModel
()
{
}

shared_ptr<Project>
DreamProjectModel::getProject
()
{
    return mProject;
}

vector<shared_ptr<SceneDefinition>>
DreamProjectModel::getScenes
()
{
    return mProject->getProjectDefinition()->getSceneDefinitionsList();
}

vector<shared_ptr<IAssetDefinition> >
DreamProjectModel::getAssetDefinitions
()
{
    return mProject->getProjectDefinition()->getAssetDefinitionsList();
}

bool
DreamProjectModel::loadProject
(QString path)
{
    mProject = make_shared<Project>(mWindowComponent);
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

shared_ptr<IAssetDefinition>
DreamProjectModel::getAssetDefinitionByUuid
(std::string uuid)
{
    return mProject
        ->getProjectDefinition()
        ->getAssetDefinitionByUuid(uuid);
}

shared_ptr<SceneDefinition>
DreamProjectModel::getSceneDefinitionByUuid
(string uuid)
{
    return getProject()->getProjectDefinition()->getSceneDefinitionByUuid(uuid);
}

bool
DreamProjectModel::startSceneRuntimeFromDefinition
(shared_ptr<SceneDefinition> definition)
{
    auto log = spdlog::get("DreamProjectModel");
    if (!definition)
    {
        log->info("No SceneDefinition selected");
        return false;
    }
    log->info("\n===== DreamModel - Start Scene =====");

    shared_ptr<ProjectRuntime> prHandle = mProject->createProjectRuntime();
    if (prHandle != nullptr)
    {
        shared_ptr<SceneRuntime> srHandle = prHandle->constructActiveSceneRuntime(definition);
        if (srHandle != nullptr)
        {
            mWindowComponent->setProjectRuntime(mProject->getProjectRuntime());
            setupHeartbeatTimer();
            return srHandle != nullptr;
        }
    }
    return false;
}

shared_ptr<SceneDefinition>
DreamProjectModel::getSelectedSceneDefinition
()
{
    return mSelectedScene;
}

void
DreamProjectModel::setSelectedSceneDefinition
(shared_ptr<SceneDefinition>  selectedScene)
{
    mSelectedScene = selectedScene;
}

void
DreamProjectModel::stopActiveSceneRuntime
()
{
    if (mProject)
    {
        shared_ptr<ProjectRuntime> prHandle = mProject->getProjectRuntime();
        if (prHandle)
        {
            shared_ptr<SceneRuntime> srHandle = prHandle->getActiveSceneRuntime();
            if (srHandle)
            {
                srHandle->setState(SCENE_STATE_STOPPED);
                prHandle->resetActiveSceneRuntime();
            }
        }

        mProject->resetProjectRuntime();

        if (mHeartbeatTimer != nullptr)
        {
            mHeartbeatTimer->stop();
            mHeartbeatTimer.reset();
        }
    }
}

void
DreamProjectModel::setDebug
(bool enabled)
{
}

void DreamProjectModel::setPhysicsDebug(bool enabled)
{
    if (mProject)
    {
        shared_ptr<ProjectRuntime> prHandle = mProject->getProjectRuntime();
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
    mProject.reset();
}

shared_ptr<IDefinition>
DreamProjectModel::createNewAssetDefinition
(AssetType type)
{
    if (mProject)
    {
        shared_ptr<ProjectDefinition> pdHandle = mProject->getProjectDefinition();
        if (pdHandle)
        {
            shared_ptr<IAssetDefinition> adHandle = pdHandle->createNewAssetDefinition(type);
            if (adHandle)
            {
                return adHandle;
            }
        }
    }
    return nullptr;
}

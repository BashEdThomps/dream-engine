/*
 * ProjectDirModel.cpp
 *
 * Created: 27 2017 by Ashley
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
#include "ProjectDirModel.h"
#include <QDebug>
#include <DreamCore.h>

using namespace Dream;

ProjectDirModel::ProjectDirModel
(QString absolutePath, QObject* parent)
    : QObject(parent),
      mAbsolutePath(absolutePath)
{
    qDebug() << "ProjectDirModel: Constructing Object";
    mAbsolutePath = absolutePath;
    populateDir();
}

bool
ProjectDirModel::populateDir
()
{
    mProjectDir.reset(new QDir(mAbsolutePath));

    if (!projectDirExists())
    {
       if (!createProjectDir())
       {
           qDebug() << "ProjectDirModel: Unable to create project directory "
                    << mAbsolutePath;
           return false;
       }
    }

    if (!assetsDirExists())
    {
        QString assetsPath = getAssetsDirAbsPath();
        if (!createAssetsDir())
        {
            qDebug() << "ProjectDirModel: Unable to create assets directory "
                    << assetsPath;
           return false;
        }
    }

    mAssetsDir.reset(new QDir(getAssetsDirAbsPath()));

    return true;
}

ProjectDirModel::~ProjectDirModel
()
{
    qDebug() << "ProjectDirModel: Destructing Object";
}

bool
ProjectDirModel::createProjectDir
()
{
    qDebug() << QString("ProjectDirModel: Creating project directory {1})").arg(mAbsolutePath);
    return QDir().mkpath(mAbsolutePath);
}

bool
ProjectDirModel::projectDirExists
()
{
    if (mProjectDir)
    {
        return mProjectDir->exists();
    }
    else
    {
        return false;
    }
}


bool
ProjectDirModel::createAssetsDir
()
{
    bool result = false;
    if (projectDirExists())
    {
        result = mProjectDir->mkdir(QString::fromStdString(Constants::ASSET_DIR));

        if (result)
        {
            mAssetsDir.reset(new QDir(getAssetsDirAbsPath()));
        }
    }
    return result;
}


bool
ProjectDirModel::createAnimationDir
()
{
    bool result = false;
    if (assetsDirExists())
    {
        result = mAssetsDir->mkdir(QString::fromStdString(Constants::ASSET_TYPE_ANIMATION));

        if (result)
        {
            mAnimationDir.reset(new QDir(getAnimationDirAbsPath()));
        }

    }
    return result;
}


bool
ProjectDirModel::createAudioDir
()
{
    bool result = false;
    if (mAssetsDir)
    {
        result = mAssetsDir->mkdir(QString::fromStdString(Constants::ASSET_TYPE_AUDIO));

        if (result)
        {
            mAnimationDir.reset(new QDir(getAnimationDirAbsPath()));
        }

    }
    return result;
}


bool
ProjectDirModel::createFontDir
()
{
    bool result = false;
    if (mAssetsDir)
    {
        result = mAssetsDir->mkdir(QString::fromStdString(Constants::ASSET_TYPE_AUDIO));

        if (result)
        {
            mAnimationDir.reset(new QDir(getAnimationDirAbsPath()));
        }

    }
    return result;
}


bool
ProjectDirModel::createModelDir
()
{
    bool result = false;
    if (mAssetsDir)
    {
        result = mAssetsDir->mkdir(QString::fromStdString(Constants::ASSET_TYPE_AUDIO));

        if (result)
        {
            mAnimationDir.reset(new QDir(getAnimationDirAbsPath()));
        }

    }
    return result;
    return false;
}


bool
ProjectDirModel::createPhysicsObjectDir
()
{
    bool result = false;
    if (mAssetsDir)
    {
        result = mAssetsDir->mkdir(QString::fromStdString(Constants::ASSET_TYPE_AUDIO));

        if (result)
        {
            mAnimationDir.reset(new QDir(getAnimationDirAbsPath()));
        }

    }
    return result;
    return false;
}


bool
ProjectDirModel::createScriptDir
()
{
    bool result = false;
    if (mAssetsDir)
    {
        result = mAssetsDir->mkdir(QString::fromStdString(Constants::ASSET_TYPE_AUDIO));

        if (result)
        {
            mAnimationDir.reset(new QDir(getAnimationDirAbsPath()));
        }

    }
    return result;
    return false;
}


bool
ProjectDirModel::createShaderDir
()
{
    bool result = false;
    if (mAssetsDir)
    {
        result = mAssetsDir->mkdir(QString::fromStdString(Constants::ASSET_TYPE_AUDIO));

        if (result)
        {
            mAnimationDir.reset(new QDir(getAnimationDirAbsPath()));
        }

    }
    return result;
    return false;
}


bool
ProjectDirModel::createSpriteDir
()
{
    bool result = false;
    if (mAssetsDir)
    {
        result = mAssetsDir->mkdir(QString::fromStdString(Constants::ASSET_TYPE_AUDIO));

        if (result)
        {
            mAnimationDir.reset(new QDir(getAnimationDirAbsPath()));
        }

    }
    return result;
    return false;
}

bool
ProjectDirModel::projectFileExists
()
{
   return false;
}

bool
ProjectDirModel::isValidProject
()
{
    bool hasAssetDir = assetsDirExists();
    bool hasProjectFile = projectFileExists();
    return hasAssetDir && hasProjectFile;
}

QString
ProjectDirModel::getAssetsDirAbsPath
()
{
    if (mProjectDir)
    {
        return mProjectDir->filePath(QString::fromStdString(Constants::ASSET_DIR));
    }

    return QString();
}

QString
ProjectDirModel::getProjectDirAbsPath
()
{
    return mAbsolutePath;
}

QString
ProjectDirModel::getAnimationDirAbsPath
()
{
    if (mAssetsDir)
    {
        return mAssetsDir->filePath(QString::fromStdString(Constants::ASSET_TYPE_ANIMATION));
    }
    else
    {
        return QString();
    }

}

QString
ProjectDirModel::getAudioDirAbsPath
()
{
    if (mAssetsDir)
    {
        return mAssetsDir->filePath(QString::fromStdString(Constants::ASSET_TYPE_ANIMATION));
    }
    else
    {
        return QString();
    }
    return QString();
}

QString
ProjectDirModel::getFontDirAbsPath
()
{
    if (mAssetsDir)
    {
        return mAssetsDir->filePath(QString::fromStdString(Constants::ASSET_TYPE_ANIMATION));
    }
    else
    {
        return QString();
    }
    return QString();
}

QString
ProjectDirModel::getModelDirAbsPath
()
{
    if (mAssetsDir)
    {
        return mAssetsDir->filePath(QString::fromStdString(Constants::ASSET_TYPE_ANIMATION));
    }
    else
    {
        return QString();
    }
    return QString();
}

QString
ProjectDirModel::getPhysicsObjectDirAbsPath
()
{
    if (mAssetsDir)
    {
        return mAssetsDir->filePath(QString::fromStdString(Constants::ASSET_TYPE_ANIMATION));
    }
    else
    {
        return QString();
    }
    return QString();
}

QString
ProjectDirModel::getScriptDirAbsPath
()
{
    if (mAssetsDir)
    {
        return mAssetsDir->filePath(QString::fromStdString(Constants::ASSET_TYPE_ANIMATION));
    }
    else
    {
        return QString();
    }
    return QString();
}

QString
ProjectDirModel::getShaderDirAbsPath
()
{
    if (mAssetsDir)
    {
        return mAssetsDir->filePath(QString::fromStdString(Constants::ASSET_TYPE_ANIMATION));
    }
    else
    {
        return QString();
    }
    return QString();
}

QString
ProjectDirModel::getSpriteDirAbsPath
()
{
    if (mAssetsDir)
    {
        return mAssetsDir->filePath(QString::fromStdString(Constants::ASSET_TYPE_ANIMATION));
    }
    else
    {
        return QString();
    }
    return QString();
}

bool
ProjectDirModel::assetsDirExists
()
{
    if (mProjectDir)
    {
        return mProjectDir->exists(QString::fromStdString(Constants::ASSET_DIR));
    }
    else
    {
        return false;
    }
}

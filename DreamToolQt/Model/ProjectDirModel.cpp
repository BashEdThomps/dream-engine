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

ProjectDirModel::ProjectDirModel
(QString absolutePath, QObject* parent)
    : QObject(parent)
{
    mAbsolutePath = absolutePath;
    mProjectDir = nullptr;
    mAssetsDir = nullptr;
    mAnimationDir = nullptr;
    mAudioDir = nullptr;
    mFontDir = nullptr;
    mModelDir = nullptr;
    mPhysicsObjetDir = nullptr;

    mShaderDir = nullptr;
    mSpriteDir = nullptr;
}

bool
ProjectDirModel::populateDir
()
{
    mProjectDir = new QDir(mAbsolutePath);

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

    mAssetsDir = new QDir(getAssetsDirAbsPath());

    return true;
}

ProjectDirModel::~ProjectDirModel
()
{
    if (mAssetsDir)
    {
        delete mAssetsDir;
        mAssetsDir = nullptr;
    }

    if (mProjectDir)
    {
        delete mProjectDir;
        mProjectDir = nullptr;
    }
}

bool
ProjectDirModel::createProjectDir
()
{
    qDebug() << QString("Creating project directory {1})").arg(mAbsolutePath);
    return QDir().mkpath(mAbsolutePath);
}

bool
ProjectDirModel::projectDirExists
()
{
    return mProjectDir->exists();
}


bool
ProjectDirModel::createAssetsDir
()
{
    bool result = false;
    if (mProjectDir)
    {
        if (mAssetsDir)
        {
            delete mAssetsDir;
            mAssetsDir = nullptr;
        }

        result = mProjectDir->mkdir(ASSET_DIR);

        if (result)
        {
            mAssetsDir = new QDir(getAssetsDirAbsPath());
        }
    }
    return result;
}


bool
ProjectDirModel::createAnimationDir
()
{
    bool result = false;
    if (mAssetsDir)
    {
        if (mAnimationDir)
        {
            delete mAnimationDir;
            mAnimationDir = nullptr;
        }

        result = mAssetsDir->mkdir(ASSET_TYPE_ANIMATION);

        if (result)
        {
            mAnimationDir = new QDir(getAnimationDirAbsPath());
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
        if (mAnimationDir)
        {
            delete mAudioDir;
            mAudioDir = nullptr;
        }

        result = mAsetsDir->mkdir(ASSET_TYPE_AUDIO);

        if (result)
        {
            mAnimationDir = new QDir(getAnimationDirAbsPath());
        }

    }
    return result;
}


bool
ProjectDirModel::createFontDir
()
{
    return false;
}


bool
ProjectDirModel::createModelDir
()
{
    return false;
}


bool
ProjectDirModel::createPhysicsObjectDir
()
{
    return false;
}


bool
ProjectDirModel::createScriptDir
()
{
    return false;
}


bool
ProjectDirModel::createShaderDir
()
{
    return false;
}


bool
ProjectDirModel::createSpriteDir
()
{
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
        return mProjectDir->filePath(ASSET_DIR);
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

}

QString
ProjectDirModel::getAudioDirAbsPath
()
{

}

QString
ProjectDirModel::getFontDirAbsPath
()
{

}

QString
ProjectDirModel::getModelDirAbsPath
()
{

}

QString
ProjectDirModel::getPhysicsObjectDirAbsPath
()
{

}

QString
ProjectDirModel::getScriptDirAbsPath
()
{

}

QString
ProjectDirModel::getShaderDirAbsPath
()
{

}

QString
ProjectDirModel::getSpriteDirAbsPath
()
{

}

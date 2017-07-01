/*
 * ProjectDirectoryModel.cpp
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
#include "ProjectDirectoryModel.h"
#include <QDebug>
#include <DreamCore.h>
#include <QDirIterator>
#include <QJsonDocument>
#include <QJsonObject>

using Dream::AssetDefinition;
using Dream::Constants;
using Dream::Uuid;

ProjectDirectoryModel::ProjectDirectoryModel
(ProjectDefinition* pdHandle, QObject* parent)
    : QObject(parent),
      mProjectDefinitionHandle(pdHandle)
{
    qDebug() << "ProjectDirectoryModel: Constructing Object";
}

ProjectDirectoryModel::~ProjectDirectoryModel
()
{
    qDebug() << "ProjectDirectoryModel: Destructing Object";
}

bool
ProjectDirectoryModel::inflateFromDirectory
(QString absolutePath)
{
    qDebug() << "ProjectDirectoryModel: Inflating from project tree in " << absolutePath;

    mAbsolutePath = absolutePath;

    mProjectDirectory = QDir(mAbsolutePath);
    if (!projectDirectoryExists())
    {
        return false;
    }

    mAssetsDirectory = QDir(getAssetsDirectoryAbsolutePath());
    mAssetsDirectory.makeAbsolute();
    if (!assetsDirectoryExists())
    {
        return false;
    }

    mAnimationDirectory = QDir(getAnimationDirectoryAbsolutePath());
    mAnimationDirectory.makeAbsolute();
    if (!animationDirectoryExists())
    {
        return false;
    }

    mAudioDirectory = QDir(getAudioDirectoryAbsolutePath());
    mAudioDirectory.makeAbsolute();
    if (!audioDirectoryExists())
    {
        return false;
    }

    mFontDirectory = QDir(getFontDirectoryAbsolutePath());
    mFontDirectory.makeAbsolute();
    if (!fontDirectoryExists())
    {
        return false;
    }

    mModelDirectory = QDir(getModelDirectoryAbsolutePath());
    mModelDirectory.makeAbsolute();
    if (!modelDirectoryExists())
    {
        return false;
    }

    mPhysicsObjetDirectory = QDir(getPhysicsObjectDirectoryAbsolutePath());
    mPhysicsObjetDirectory.makeAbsolute();
    if (!physicsObjectDirectoryExists())
    {
        return false;
    }

    mScriptDirectory = QDir(getScriptDirectoryAbsolutePath());
    mScriptDirectory.makeAbsolute();
    if (!scriptDirectoryExists())
    {
        return false;
    }

    mShaderDirectory = QDir(getShaderDirectoryAbsolutePath());
    mShaderDirectory.makeAbsolute();
    if (!shaderDirectoryExists())
    {
        return false;
    }

    mSpriteDirectory = QDir(getSpriteDirectoryAbsolutePath());
    mSpriteDirectory.makeAbsolute();
    if (!spriteDirectoryExists())
    {
        return false;
    }

    if (!projectFileExists())
    {
        return false;
    }

    return true;
}


bool
ProjectDirectoryModel::createNewProjectTree
(QString absolutePath)
{
    qDebug() << "ProjectDirectoryModel: Creating new project tree in " << absolutePath;
    mAbsolutePath = absolutePath;
    mProjectDirectory = QDir(mAbsolutePath);

    if (!projectDirectoryExists())
    {
        if (!createProjectDirectory())
        {
            qDebug() << "ProjectDirectoryModel: Unable to create directory "
                     << mAbsolutePath;
            return false;
        }
    }

    if (!assetsDirectoryExists())
    {
        if (!createAssetsDirectory())
        {
            qDebug() << "ProjectDirectoryModel: Unable to create directory "
                     << getAssetsDirectoryAbsolutePath();
            return false;
        }
    }

    if (!animationDirectoryExists())
    {
        if (!createAnimationDirectory())
        {
            qDebug() << "ProjectDirectoryModel: Unable to create directory "
                     << getAnimationDirectoryAbsolutePath();
            return false;
        }
    }

    if (!audioDirectoryExists())
    {
        if (!createAudioDirectory())
        {
            qDebug() << "ProjectDirectoryModel: Unable to create directory "
                     << getAnimationDirectoryAbsolutePath();
            return false;
        }
    }

    if (!fontDirectoryExists())
    {
        if (!createFontDirectory())
        {
            qDebug() << "ProjectDirectoryModel: Unable to create directory "
                     << getFontDirectoryAbsolutePath();
            return false;
        }
    }

    if (!modelDirectoryExists())
    {
        if (!createModelDirectory())
        {
            qDebug() << "ProjectDirectoryModel: Unable to create directory "
                     << getModelDirectoryAbsolutePath();
            return false;
        }
    }

    if (!physicsObjectDirectoryExists())
    {
        if (!createPhysicsObjectDirectory())
        {
            qDebug() << "ProjectDirectoryModel: Unable to create directory "
                     << getPhysicsObjectDirectoryAbsolutePath();
            return false;
        }
    }

    if (!scriptDirectoryExists())
    {
        if (!createScriptDirectory())
        {
            qDebug() << "ProjectDirectoryModel: Unable to create directory "
                     << getScriptDirectoryAbsolutePath();
            return false;
        }
    }

    if (!shaderDirectoryExists())
    {
        if (!createShaderDirectory())
        {
            qDebug() << "ProjectDirectoryModel: Unable to create directory "
                     << getShaderDirectoryAbsolutePath();
            return false;
        }
    }

    if (!spriteDirectoryExists())
    {
        if (!createSpriteDirectory())
        {
            qDebug() << "ProjectDirectoryModel: Unable to create directory "
                     << getSpriteDirectoryAbsolutePath();
            return false;
        }
    }

    return true;
}

bool ProjectDirectoryModel::writeProjectFile()
{
    if (!mProjectDefinitionHandle)
    {
        qDebug() << "ProjectDirectoryModel: No ProjectDefinition set";
        return false;
    }

    QByteArray projectFileData = mProjectDefinitionHandle->getJson().dump(1).c_str();

    qDebug() << "ProjectDirectoryModel: writing project file to "
             << getProjectFileAbsolutePath();

    QFile projectFile(getProjectFileAbsolutePath());

    if (!projectFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
       qDebug() << "ProjectDirectoryModel: unable to open project file for modification";
       return false;
    }

    projectFile.setPermissions(QFileDevice::Permission::ReadOwner | QFileDevice::Permission::WriteOwner);

    bool result = projectFile.write(projectFileData);
    projectFile.close();

    return result;
}

bool
ProjectDirectoryModel::createProjectDirectory
()
{
    qDebug() << QString("ProjectDirectoryModel: Creating project directory {1})").arg(mAbsolutePath);
    return QDir().mkpath(mAbsolutePath);
}

bool
ProjectDirectoryModel::projectDirectoryExists
()
{
    return mProjectDirectory.exists();
}


bool
ProjectDirectoryModel::createAssetsDirectory
()
{
    bool result = false;
    if (projectDirectoryExists())
    {
        result = mProjectDirectory.mkdir(QString::fromStdString(Constants::ASSET_DIR));

        if (result)
        {
            mAssetsDirectory = QDir(getAssetsDirectoryAbsolutePath());
        }
    }
    return result;
}


bool
ProjectDirectoryModel::createAnimationDirectory
()
{
    bool result = false;
    if (assetsDirectoryExists())
    {
        result = mAssetsDirectory.mkdir(QString::fromStdString(Constants::ASSET_TYPE_ANIMATION));

        if (result)
        {
            mAnimationDirectory = QDir(getAnimationDirectoryAbsolutePath());
        }

    }
    return result;
}


bool
ProjectDirectoryModel::createAudioDirectory
()
{
    bool result = false;
    if (assetsDirectoryExists())
    {
        result = mAssetsDirectory.mkdir(QString::fromStdString(Constants::ASSET_TYPE_AUDIO));

        if (result)
        {
            mAnimationDirectory = QDir(getAnimationDirectoryAbsolutePath());
        }

    }
    return result;
}


bool
ProjectDirectoryModel::createFontDirectory
()
{
    bool result = false;
    if (assetsDirectoryExists())
    {
        result = mAssetsDirectory.mkdir(QString::fromStdString(Constants::ASSET_TYPE_FONT));

        if (result)
        {
            mAnimationDirectory = QDir(getAnimationDirectoryAbsolutePath());
        }

    }
    return result;
}


bool
ProjectDirectoryModel::createModelDirectory
()
{
    bool result = false;
    if (assetsDirectoryExists())
    {
        result = mAssetsDirectory.mkdir(QString::fromStdString(Constants::ASSET_TYPE_MODEL));

        if (result)
        {
            mAnimationDirectory = QDir(getAnimationDirectoryAbsolutePath());
        }

    }
    return result;
}


bool
ProjectDirectoryModel::createPhysicsObjectDirectory
()
{
    bool result = false;
    if (assetsDirectoryExists())
    {
        result = mAssetsDirectory.mkdir(QString::fromStdString(Constants::ASSET_TYPE_PHYSICS_OBJECT));

        if (result)
        {
            mAnimationDirectory = QDir(getAnimationDirectoryAbsolutePath());
        }

    }
    return result;
}


bool
ProjectDirectoryModel::createScriptDirectory
()
{
    bool result = false;
    if (assetsDirectoryExists())
    {
        result = mAssetsDirectory.mkdir(QString::fromStdString(Constants::ASSET_TYPE_SCRIPT));

        if (result)
        {
            mAnimationDirectory = QDir(getAnimationDirectoryAbsolutePath());
        }

    }
    return result;
}


bool
ProjectDirectoryModel::createShaderDirectory
()
{
    bool result = false;
    if (assetsDirectoryExists())
    {
        result = mAssetsDirectory.mkdir(QString::fromStdString(Constants::ASSET_TYPE_SHADER));

        if (result)
        {
            mAnimationDirectory = QDir(getAnimationDirectoryAbsolutePath());
        }

    }
    return result;
}


bool
ProjectDirectoryModel::createSpriteDirectory
()
{
    bool result = false;
    if (assetsDirectoryExists())
    {
        result = mAssetsDirectory.mkdir(QString::fromStdString(Constants::ASSET_TYPE_SPRITE));

        if (result)
        {
            mAnimationDirectory = QDir(getAnimationDirectoryAbsolutePath());
        }

    }
    return result;
}

bool
ProjectDirectoryModel::projectFileExists
()
{
    if (projectDirectoryExists())
    {
        QString jsonSuffix = QString::fromStdString(Constants::PROJECT_EXTENSION);
        QDirIterator it(mProjectDirectory, QDirIterator::NoIteratorFlags);
        QFileInfo projInfo(getProjectDirectoryAbsolutePath());

        while (it.hasNext())
        {
            QString filename = it.next();
            QFileInfo file(filename);
            if (file.isFile() &&
                file.baseName() == projInfo.fileName() &&
                file.completeSuffix().remove("\"") == jsonSuffix.remove("."))
            {
                qDebug() << "ProjectDirectoryModel: Found Project File: " << file.fileName();
                return true;
            }
        }
    }
    return false;
}

bool
ProjectDirectoryModel::isValidProject
()
{
    bool hasAssetDir = assetsDirectoryExists();
    bool hasProjectFile = projectFileExists();
    return hasAssetDir && hasProjectFile;
}

QString
ProjectDirectoryModel::getAssetsDirectoryAbsolutePath
()
{
    if (projectDirectoryExists())
    {
        return mProjectDirectory.filePath(QString::fromStdString(Constants::ASSET_DIR));
    }
    return QString();
}

QString
ProjectDirectoryModel::getProjectDirectoryAbsolutePath
()
{
    return mAbsolutePath;
}

QString
ProjectDirectoryModel::getAnimationDirectoryAbsolutePath
()
{
    if (assetsDirectoryExists())
    {
        return mAssetsDirectory.filePath(QString::fromStdString(Constants::ASSET_TYPE_ANIMATION));
    }
    return QString();
}

QString
ProjectDirectoryModel::getAudioDirectoryAbsolutePath
()
{
    if (assetsDirectoryExists())
    {
        return mAssetsDirectory.filePath(QString::fromStdString(Constants::ASSET_TYPE_AUDIO));
    }
    return QString();
}

QString
ProjectDirectoryModel::getFontDirectoryAbsolutePath
()
{
    if (assetsDirectoryExists())
    {
        return mAssetsDirectory.filePath(QString::fromStdString(Constants::ASSET_TYPE_FONT));
    }
    return QString();
}

QString
ProjectDirectoryModel::getModelDirectoryAbsolutePath
()
{
    if (assetsDirectoryExists())
    {
        return mAssetsDirectory.filePath(QString::fromStdString(Constants::ASSET_TYPE_MODEL));
    }
    return QString();
}

QString
ProjectDirectoryModel::getPhysicsObjectDirectoryAbsolutePath
()
{
    if (assetsDirectoryExists())
    {
        return mAssetsDirectory.filePath(QString::fromStdString(Constants::ASSET_TYPE_PHYSICS_OBJECT));
    }
    return QString();
}

QString
ProjectDirectoryModel::getScriptDirectoryAbsolutePath
()
{
    if (assetsDirectoryExists())
    {
        return mAssetsDirectory.filePath(QString::fromStdString(Constants::ASSET_TYPE_SCRIPT));
    }
    return QString();
}

QString
ProjectDirectoryModel::getShaderDirectoryAbsolutePath
()
{
    if (assetsDirectoryExists())
    {
        return mAssetsDirectory.filePath(QString::fromStdString(Constants::ASSET_TYPE_SHADER));
    }
    return QString();
}

QString
ProjectDirectoryModel::getSpriteDirectoryAbsolutePath
()
{
    if (assetsDirectoryExists())
    {
        return mAssetsDirectory.filePath(QString::fromStdString(Constants::ASSET_TYPE_SPRITE));
    }
    return QString();
}

QString
ProjectDirectoryModel::getProjectFileAbsolutePath
()
{
    QFileInfo info(getProjectDirectoryAbsolutePath());
    return mProjectDirectory.filePath(info.fileName()+QString::fromStdString(Constants::PROJECT_EXTENSION));
}

QString
ProjectDirectoryModel::getProjectDirectoryName
()
{
    return mProjectDirectory.dirName();
}

bool
ProjectDirectoryModel::assetMainFileExists
(AssetDefinition *adHandle, string format)
{
    QString assetFileTargetPath = createAssetTargetPath(adHandle,format);
    return QFile(assetFileTargetPath).exists();
}

bool ProjectDirectoryModel::deleteMainAssetFile(Dream::AssetDefinition *adHandle, string format)
{
    QString assetFileTargetPath = createAssetTargetPath(adHandle,format);
    return QFile(assetFileTargetPath).remove();
}

bool
ProjectDirectoryModel::assetsDirectoryExists
()
{
    if (projectDirectoryExists())
    {
        return mProjectDirectory.exists(QString::fromStdString(Constants::ASSET_DIR));
    }
    return false;
}

bool
ProjectDirectoryModel::audioDirectoryExists
()
{
    if (assetsDirectoryExists())
    {
        return mAssetsDirectory.exists(QString::fromStdString(Constants::ASSET_TYPE_AUDIO));
    }
    return false;
}

bool
ProjectDirectoryModel::animationDirectoryExists
()
{
    if (assetsDirectoryExists())
    {
        return mAssetsDirectory.exists(QString::fromStdString(Constants::ASSET_TYPE_ANIMATION));
    }
    return false;
}

bool
ProjectDirectoryModel::fontDirectoryExists
()
{
    if (assetsDirectoryExists())
    {
        return mAssetsDirectory.exists(QString::fromStdString(Constants::ASSET_TYPE_AUDIO));
    }
    return false;
}

bool
ProjectDirectoryModel::modelDirectoryExists
()
{
    if (assetsDirectoryExists())
    {
        return mAssetsDirectory.exists(QString::fromStdString(Constants::ASSET_TYPE_MODEL));
    }
    return false;
}

bool
ProjectDirectoryModel::physicsObjectDirectoryExists
()
{
    if (assetsDirectoryExists())
    {
        return mAssetsDirectory.exists(QString::fromStdString(Constants::ASSET_TYPE_PHYSICS_OBJECT));
    }
    return false;
}

bool
ProjectDirectoryModel::scriptDirectoryExists
()
{
    if (assetsDirectoryExists())
    {
        return mAssetsDirectory.exists(QString::fromStdString(Constants::ASSET_TYPE_SCRIPT));
    }
    return false;
}

bool
ProjectDirectoryModel::shaderDirectoryExists
()
{
    if (assetsDirectoryExists())
    {
        return mAssetsDirectory.exists(QString::fromStdString(Constants::ASSET_TYPE_SHADER));
    }
    return false;
}

bool
ProjectDirectoryModel::spriteDirectoryExists
()
{
    if (assetsDirectoryExists())
    {
        return mAssetsDirectory.exists(QString::fromStdString(Constants::ASSET_TYPE_SPRITE));
    }
    return false;
}

QString
ProjectDirectoryModel::getProjectName
()
{
    return  QFileInfo(getProjectDirectoryAbsolutePath()).fileName();
}

QString
ProjectDirectoryModel::createAssetTargetPath
(AssetDefinition* adHandle, string format)
{
    QString assetFileTargetPath;

    // Get type directory
    assetFileTargetPath = mAssetsDirectory.filePath(
        QString::fromStdString(adHandle->getAssetTypeDirectory())
    );

    // Create uuid directory
    QDir typeDir(assetFileTargetPath);
    if (!typeDir.mkpath(QString::fromStdString(adHandle->getUuid())))
    {
        qDebug() << "ProjectDirectoryModel: Error could not create"
                 << QString::fromStdString(adHandle->getUuid())
                 << "in"
                 << assetFileTargetPath;

        return QString();
    }

    // Update to add uuid to path
    assetFileTargetPath = QDir(assetFileTargetPath).filePath(QString::fromStdString(adHandle->getUuid()));

    qDebug() << "ProjectDirectoryModel: Created path " << assetFileTargetPath;

    // Update to add "Format" to path
    if (format.empty())
    {
        assetFileTargetPath = QDir(assetFileTargetPath).filePath(
            QString::fromStdString(adHandle->getFormat())
        );
    }
    else
    {
        assetFileTargetPath = QDir(assetFileTargetPath).filePath(
            QString::fromStdString(format)
        );
    }

    return assetFileTargetPath;
}

bool
ProjectDirectoryModel::copyMainAssetFile
(AssetDefinition* adHandle, QFile& assetSourceFile, string format)
{
    QString assetFileTargetPath = createAssetTargetPath(adHandle,format);

    qDebug() << "ProjectDirectoryModel: Copying asset file for "
         << QString::fromStdString(adHandle->getNameAndUuidString())
         << ", with format "
         << QString::fromStdString(adHandle->getFormat())
         << " from "
         << assetSourceFile.fileName()
         << " to "
         << assetFileTargetPath;

    bool copyResult = assetSourceFile.copy(assetFileTargetPath);
    if (!copyResult)
    {
        qDebug() << "ProjectDirectoryModel: Coyping error" << assetSourceFile.errorString();
    }
    return copyResult;
}

bool
ProjectDirectoryModel::copyAdditionalFile
(AssetDefinition* adHandle, QFile& assetSourceFile)
{
    QString assetFileTargetPath = createAssetTargetPath
    (
        adHandle,
        QFileInfo(assetSourceFile.fileName()).fileName().toStdString()
    );

    qDebug() << "ProjectDirectoryModel: Copying asset file for "
         << QString::fromStdString(adHandle->getNameAndUuidString())
         << " from "
         << assetSourceFile.fileName()
         << " to "
         << assetFileTargetPath;

    bool copyResult = assetSourceFile.copy(assetFileTargetPath);
    if (!copyResult)
    {
        qDebug() << "ProjectDirectoryModel: Coyping error" << assetSourceFile.errorString();
    }
    return copyResult;
}

ProjectDefinition*
ProjectDirectoryModel::getProjectDefinitionHandle
()
{
    return mProjectDefinitionHandle;
}

void
ProjectDirectoryModel::setProjectDefinitionHandle
(ProjectDefinition *projectDefinitionHandle)
{
    mProjectDefinitionHandle = projectDefinitionHandle;
}

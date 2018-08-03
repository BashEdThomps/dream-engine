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
#include "spdlog/spdlog.h"

using Dream::IAssetDefinition;
using Dream::Constants;
using Dream::Uuid;

ProjectDirectoryModel::ProjectDirectoryModel
(ProjectDefinition* pdHandle, QObject* parent)
    : QObject(parent),
      mProjectDefinitionHandle(pdHandle)
{
    auto log = spdlog::get("ProjectDirectoryModel");
    if (log == nullptr)
    {
        log = spdlog::stdout_color_mt("ProjectDirectoryModel");
    }
    log->info("Constructing");

}

ProjectDirectoryModel::~ProjectDirectoryModel
()
{
    auto log = spdlog::get("ProjectDirectoryModel");
    log->info("Destructing");
}

bool
ProjectDirectoryModel::inflateFromDirectory
(QString absolutePath)
{
    auto log = spdlog::get("ProjectDirectoryModel");
    log->info("Inflating from project tree in {}", absolutePath.toStdString());

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
    auto log = spdlog::get("ProjectDirectoryModel");
    log->info("Creating new project tree in {}", absolutePath.toStdString());
    mAbsolutePath = absolutePath;
    mProjectDirectory = QDir(mAbsolutePath);

    if (!projectDirectoryExists())
    {
        if (!createProjectDirectory())
        {
            log->info("Unable to create directory {}", mAbsolutePath.toStdString());
            return false;
        }
    }

    if (!assetsDirectoryExists())
    {
        if (!createAssetsDirectory())
        {
            log->info("Unable to create directory {}",getAssetsDirectoryAbsolutePath().toStdString());
            return false;
        }
    }

    if (!animationDirectoryExists())
    {
        if (!createAnimationDirectory())
        {
            log->info("Unable to create directory {}",getAnimationDirectoryAbsolutePath().toStdString());
            return false;
        }
    }

    if (!audioDirectoryExists())
    {
        if (!createAudioDirectory())
        {
            log->info("Unable to create directory {}",getAnimationDirectoryAbsolutePath().toStdString());
            return false;
        }
    }

    if (!fontDirectoryExists())
    {
        if (!createFontDirectory())
        {
            log->info("Unable to create directory {}",getFontDirectoryAbsolutePath().toStdString());
            return false;
        }
    }

    if (!modelDirectoryExists())
    {
        if (!createModelDirectory())
        {
            log->info("Unable to create directory {}",getModelDirectoryAbsolutePath().toStdString());
            return false;
        }
    }

    if (!physicsObjectDirectoryExists())
    {
        if (!createPhysicsObjectDirectory())
        {
            log->info("Unable to create directory {}",getPhysicsObjectDirectoryAbsolutePath().toStdString());
            return false;
        }
    }

    if (!scriptDirectoryExists())
    {
        if (!createScriptDirectory())
        {
            log->info("Unable to create directory {}", getScriptDirectoryAbsolutePath().toStdString());
            return false;
        }
    }

    if (!shaderDirectoryExists())
    {
        if (!createShaderDirectory())
        {
            log->info("Unable to create directory {}",getShaderDirectoryAbsolutePath().toStdString());
            return false;
        }
    }

    if (!spriteDirectoryExists())
    {
        if (!createSpriteDirectory())
        {
            log->info("Unable to create directory {}",getSpriteDirectoryAbsolutePath().toStdString());
            return false;
        }
    }

    return true;
}

bool ProjectDirectoryModel::writeProjectFile()
{
    auto log = spdlog::get("ProjectDirectoryModel");
    if (!mProjectDefinitionHandle)
    {
        log->info("No ProjectDefinition set");
        return false;
    }

    QByteArray projectFileData = mProjectDefinitionHandle->getJson().dump(1).c_str();

    log->info("Writing project file to {}",getProjectFileAbsolutePath().toStdString());

    QFile projectFile(getProjectFileAbsolutePath());

    if (!projectFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        log->info("Unable to open project file for modification");
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
    auto log = spdlog::get("ProjectDirectoryModel");
    log->info("Creating project directory {}",mAbsolutePath.toStdString());
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
    auto log = spdlog::get("ProjectDirectoryModel");
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
                log->info("Found Project File: {}", file.fileName().toStdString());
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
(IAssetDefinition *adHandle, QString format)
{
    QString assetFileTargetPath = createAssetTargetPath(adHandle,format);
    return QFile(assetFileTargetPath).exists();
}

bool ProjectDirectoryModel::deleteMainAssetFile(IAssetDefinition *adHandle, QString format)
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
ProjectDirectoryModel::getAssetDataPath
(IAssetDefinition* adHandle)
{
    QString assetDataPath;

    // Get type directory
    assetDataPath = mAssetsDirectory.filePath
            (
                QString::fromStdString(adHandle->getAssetTypeDirectory())
                );

    // Get uuid directory
    QDir typeDir(assetDataPath);
    assetDataPath = typeDir.filePath
            (
                QString::fromStdString(adHandle->getUuid())
                );

    return assetDataPath;
}

void
ProjectDirectoryModel::touchFile
(QString assetFileTargetPath)
{
    auto log = spdlog::get("ProjectDirectoryModel");
    log->info("Touching file" , assetFileTargetPath.toStdString());
    QFile newFile(assetFileTargetPath);
    newFile.open(QIODevice::ReadWrite);
    newFile.close();
}

bool
ProjectDirectoryModel::writeAssetData
(QString data, IAssetDefinition* adHandle, QString fileName, bool overwrite)
{
    bool retval = false;
    QString absPath = createAssetTargetPath(adHandle,fileName);
    QFile file(absPath);

    if (!file.exists() || overwrite)
    {
        file.open(QIODevice::ReadWrite | QIODevice::Truncate);
        retval = file.write(data.toUtf8());
        file.close();
    }
    return retval;
}

QByteArray ProjectDirectoryModel::readScriptData(Dream::ScriptDefinition* scriptDef)
{
    auto log = spdlog::get("ProjectDirectoryModel");
    log->info( "readScriptData for {}", scriptDef->getNameAndUuidString());
    QString filePath = createAssetTargetPath(scriptDef);
    touchFile(filePath);

    log->info("Opening Lua Script {}",filePath.toStdString());
    QFile scriptFile(filePath);
    scriptFile.open(QFile::ReadOnly);
    QByteArray data = scriptFile.readAll();
    scriptFile.close();
    return data;
}

ShaderFileTuple ProjectDirectoryModel::readShaderData(Dream::ShaderDefinition* shaderDef)
{
    ShaderFileTuple shaderData;
    auto log = spdlog::get("ProjectDirectoryModel");
    log->info( "readShaderData for {}", shaderDef->getNameAndUuidString());

    QString vertexFilePath = createAssetTargetPath(shaderDef, QString::fromStdString(Constants::SHADER_VERTEX_FILE_NAME));
    touchFile(vertexFilePath);
    log->info("Opening Vertex Shader {}",vertexFilePath.toStdString());
    QFile vertexFile(vertexFilePath);
    vertexFile.open(QFile::ReadOnly);
    shaderData.vertexShader = vertexFile.readAll();
    vertexFile.close();

    QString fragmentFilePath = createAssetTargetPath(shaderDef, QString::fromStdString(Constants::SHADER_FRAGMENT_FILE_NAME));
    touchFile(fragmentFilePath);
    log->info("Opening Fragment Shader {}",fragmentFilePath.toStdString());
    QFile fragmentFile(fragmentFilePath);
    fragmentFile.open(QFile::ReadOnly);
    shaderData.fragmentShader = fragmentFile.readAll();
    fragmentFile.close();

    return shaderData;
}

QString
ProjectDirectoryModel::createAssetTargetPath
(IAssetDefinition* adHandle, QString format)
{
    QString assetFileTargetPath;
    auto log = spdlog::get("ProjectDirectoryModel");

    // Get type directory
    assetFileTargetPath = mAssetsDirectory.filePath(
                QString::fromStdString(adHandle->getAssetTypeDirectory())
                );

    // Create uuid directory
    QDir typeDir(assetFileTargetPath);
    if (!typeDir.mkpath(QString::fromStdString(adHandle->getUuid())))
    {
        log->error("Error could not create {} in {}",adHandle->getUuid(),assetFileTargetPath.toStdString());
        return QString();
    }

    // Update to add uuid to path
    assetFileTargetPath = QDir(assetFileTargetPath).filePath(QString::fromStdString(adHandle->getUuid()));

    log->info("Created path ", assetFileTargetPath.toStdString());

    // Update to add "Format" to path
    if (format.isEmpty())
    {
        assetFileTargetPath = QDir(assetFileTargetPath).filePath(
                    QString::fromStdString(adHandle->getFormat())
                    );
    }
    else
    {
        assetFileTargetPath = QDir(assetFileTargetPath).filePath(format);
    }

    return assetFileTargetPath;
}

bool
ProjectDirectoryModel::copyMainAssetFile
(IAssetDefinition* adHandle, QFile& assetSourceFile, QString format)
{
    auto log = spdlog::get("ProjectDirectoryModel");
    QString assetFileTargetPath = createAssetTargetPath(adHandle,format);

    log->info(
         "Copying asset file for {}, with format {} from {} to {}",
         adHandle->getNameAndUuidString(),
         adHandle->getFormat(),
         assetSourceFile.fileName().toStdString(),
         assetFileTargetPath.toStdString()
    );

    bool copyResult = assetSourceFile.copy(assetFileTargetPath);
    if (!copyResult)
    {
        log->error("Coyping error {}" ,assetSourceFile.errorString().toStdString());
    }
    return copyResult;
}

bool
ProjectDirectoryModel::deleteAssetDataDirectory
(IAssetDefinition* adHandle)
{

    auto log = spdlog::get("ProjectDirectoryModel");
    QString assetDataPath = getAssetDataPath(adHandle);
    log->info( "ProjectDirectoryModel: Deleting Asset Data Directory {}",assetDataPath.toStdString());
    return QDir(assetDataPath).removeRecursively();
}

bool
ProjectDirectoryModel::copyAdditionalFile
(IAssetDefinition* adHandle, QFile& assetSourceFile)
{
    auto log = spdlog::get("ProjectDirectoryModel");
    QString assetFileTargetPath = createAssetTargetPath
            (
                adHandle,
                QFileInfo(assetSourceFile.fileName()).fileName()
                );

    log->info("Copying asset file for {} from {} to {}",
              adHandle->getNameAndUuidString(),
              assetSourceFile.fileName().toStdString(),
              assetFileTargetPath.toStdString());

    bool copyResult = assetSourceFile.copy(assetFileTargetPath);
    if (!copyResult)
    {
        log->error("Coyping error {}" ,assetSourceFile.errorString().toStdString());
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

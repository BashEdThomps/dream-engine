/*
 * ProjectDirModel.h
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

#pragma once

#include <memory>
#include <QObject>
#include <QDir>

using namespace std;

namespace Dream
{
    class AssetDefinition;
    class ProjectDefinition;
}

using Dream::AssetDefinition;
using Dream::ProjectDefinition;

class ProjectDirectoryModel : public QObject
{
    Q_OBJECT
public:
    ProjectDirectoryModel(ProjectDefinition* pdHanndle = nullptr, QObject* parent = nullptr);
    ~ProjectDirectoryModel();

    bool createProjectDirectory();
    bool createAssetsDirectory();
    bool createAnimationDirectory();
    bool createAudioDirectory();
    bool createFontDirectory();
    bool createModelDirectory();
    bool createPhysicsObjectDirectory();
    bool createScriptDirectory();
    bool createShaderDirectory();
    bool createSpriteDirectory();
    bool writeProjectFile();
    bool inflateFromDirectory(QString absolutePath);
    bool createNewProjectTree(QString absolutePath);

    bool projectDirectoryExists();
    bool assetsDirectoryExists();
    bool audioDirectoryExists();
    bool animationDirectoryExists();
    bool fontDirectoryExists();
    bool modelDirectoryExists();
    bool physicsObjectDirectoryExists();
    bool scriptDirectoryExists();
    bool shaderDirectoryExists();
    bool spriteDirectoryExists();

    bool projectFileExists();
    bool isValidProject();

    QString getProjectDirectoryAbsolutePath();
    QString getAssetsDirectoryAbsolutePath();
    QString getAnimationDirectoryAbsolutePath();
    QString getAudioDirectoryAbsolutePath();
    QString getFontDirectoryAbsolutePath();
    QString getModelDirectoryAbsolutePath();
    QString getPhysicsObjectDirectoryAbsolutePath();
    QString getScriptDirectoryAbsolutePath();
    QString getShaderDirectoryAbsolutePath();
    QString getSpriteDirectoryAbsolutePath();
    QString getProjectFileAbsolutePath();

    bool assetMainFileExists(AssetDefinition *adHandle, string format = "" );
    bool deleteMainAssetFile(AssetDefinition *adHandle, string format = "" );
    bool copyMainAssetFile(AssetDefinition *adHandle, QFile& assetFile, string format = "" );

    bool copyAdditionalFile(AssetDefinition *adHandle, QFile& assetFile);

    ProjectDefinition *getProjectDefinitionHandle();
    void setProjectDefinitionHandle(ProjectDefinition *projectDefinitionHandle);

private: // Variables
    ProjectDefinition* mProjectDefinitionHandle;
    QString mAbsolutePath;
    QDir mProjectDirectory;
    QDir mAssetsDirectory;
    QDir mAnimationDirectory;
    QDir mAudioDirectory;
    QDir mFontDirectory;
    QDir mModelDirectory;
    QDir mPhysicsObjetDirectory;
    QDir mScriptDirectory;
    QDir mShaderDirectory;
    QDir mSpriteDirectory;
    QString getProjectName();
private: // Methods
    QString createAssetTargetPath(AssetDefinition *adHandle, string format = "");
};

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
    class IAssetDefinition;
    class ProjectDefinition;
    class ScriptDefinition;
    class ShaderDefinition;
}

using Dream::IAssetDefinition;
using Dream::ProjectDefinition;
using Dream::ScriptDefinition;
using Dream::ShaderDefinition;

typedef struct
{
    QByteArray vertexShader;
    QByteArray fragmentShader;
} ShaderFileTuple;

class ProjectDirectoryModel : public QObject
{
    Q_OBJECT
public:
    ProjectDirectoryModel(shared_ptr<ProjectDefinition> pdHanndle = nullptr, QObject* parent = nullptr);
    ~ProjectDirectoryModel() override;

    bool createProjectDirectory();
    bool createAssetsDirectory();
    bool createPathDirectory();
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
    bool pathDirectoryExists();
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
    QString getPathDirectoryAbsolutePath();
    QString getAudioDirectoryAbsolutePath();
    QString getFontDirectoryAbsolutePath();
    QString getModelDirectoryAbsolutePath();
    QString getPhysicsObjectDirectoryAbsolutePath();
    QString getScriptDirectoryAbsolutePath();
    QString getShaderDirectoryAbsolutePath();
    QString getSpriteDirectoryAbsolutePath();
    QString getProjectFileAbsolutePath();
    QString getProjectDirectoryName();

    bool assetMainFileExists(shared_ptr<IAssetDefinition> adHandle, QString format = "" );
    bool deleteMainAssetFile(shared_ptr<IAssetDefinition> adHandle, QString format = "" );
    bool copyMainAssetFile(shared_ptr<IAssetDefinition> adHandle, QFile& assetFile, QString format = "" );
    bool deleteAssetDataDirectory(shared_ptr<IAssetDefinition> adHandle);
    bool copyAdditionalFile(shared_ptr<IAssetDefinition> adHandle, QFile& assetFile);

    shared_ptr<ProjectDefinition> getProjectDefinition();
    void setProjectDefinitionHandle(shared_ptr<ProjectDefinition> projectDefinitionHandle);

    QString createAssetTargetPath(shared_ptr<IAssetDefinition> adHandle, QString format = "");
    QString getAssetDataPath(shared_ptr<IAssetDefinition> adHandle);

    void touchFile(QString filePath);
    bool writeAssetData(QString data, shared_ptr<IAssetDefinition> definition,QString fileName = "", bool overwrite = true);

    QByteArray readScriptData(shared_ptr<ScriptDefinition> scriptDef);
    ShaderFileTuple readShaderData(shared_ptr<ShaderDefinition> shaderDef);

private: // Variables
    shared_ptr<ProjectDefinition> mProjectDefinitionHandle;
    QString mAbsolutePath;
    QDir mProjectDirectory;
    QDir mAssetsDirectory;
    QDir mPathDirectory;
    QDir mAudioDirectory;
    QDir mFontDirectory;
    QDir mModelDirectory;
    QDir mPhysicsObjetDirectory;
    QDir mScriptDirectory;
    QDir mShaderDirectory;
    QDir mSpriteDirectory;
    QString getProjectName();

};



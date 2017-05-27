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
#ifndef PROJECTDIRMODEL_H
#define PROJECTDIRMODEL_H

#include <QObject>
#include <QDir>

class ProjectDirModel : public QObject
{
    Q_OBJECT
public:
    ProjectDirModel(QString absolutePath, QObject* parent);
    ~ProjectDirModel();

    bool createProjectDir();
    bool createAssetsDir();
    bool createAnimationDir();
    bool createAudioDir();
    bool createFontDir();
    bool createModelDir();
    bool createPhysicsObjectDir();
    bool createScriptDir();
    bool createShaderDir();
    bool createSpriteDir();
    bool populateDir();

    bool projectDirExists();
    bool projectFileExists();
    bool assetsDirExists();

    bool isValidProject();

    QString getProjectDirAbsPath();
    QString getAssetsDirAbsPath();
    QString getAnimationDirAbsPath();
    QString getAudioDirAbsPath();
    QString getFontDirAbsPath();
    QString getModelDirAbsPath();
    QString getPhysicsObjectDirAbsPath();
    QString getScriptDirAbsPath();
    QString getShaderDirAbsPath();
    QString getSpriteDirAbsPath();

private:
    QString mAbsolutePath;
    QDir *mProjectDir;
    QDir *mAssetsDir;
    QDir *mAnimationDir;
    QDir *mAudioDir;
    QDir *mFontDir;
    QDir *mModelDir;
    QDir *mPhysicsObjetDir;
    QDir *mScriptDir;
    QDir *mShaderDir;
    QDir *mSpriteDir;
};


#endif // PROJECTDIRMODEL_H

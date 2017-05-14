/*
 * DreamModel.cpp
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
#include "DreamModel.h"

DreamModel::DreamModel(QObject *parent) : QObject(parent)
{
    mDreamEngine = new Dream::DreamEngine<QTDreamAudioComponent,QTDreamWindowComponent>();
}

DreamModel::~DreamModel()
{

}

Dream::Project* DreamModel::getProject()
{
    if (mDreamEngine)
    {
        return mDreamEngine->getProject();
    }
    return nullptr;
}

vector<Dream::Scene*>* DreamModel::getScenes()
{
    if (mDreamEngine)
    {
        return nullptr;
    }
    return nullptr;
}

vector<Dream::AssetDefinition*>* DreamModel::getAssetDefinitions()
{
    if (mDreamEngine)
    {
        return nullptr;
    }
    return nullptr;
}


bool DreamModel::loadProject(QString path)
{
    return mDreamEngine->loadFromDirectory(path.toStdString());
}

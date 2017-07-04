/*
 * MacOSOpenModel.cpp
 *
 * Created: 04 2017 by Ashley
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
#include "MacOSOpenModel.h"

#include "PreferencesModel.h"
#include <QDebug>

MacOSOpenModel::MacOSOpenModel
(PreferencesModel* pmHandle, QObject *parent)
    : QObject(parent),
      mPreferencesModelHandle(pmHandle)
{

}

void
MacOSOpenModel::openInExternalEditor
(QString filePath)
{
    QProcess *process = new QProcess(this);

    QString program = "open";

    QStringList arguments;
    arguments << filePath
              << "-a"
              << mPreferencesModelHandle->getExternalTextEditorPath();
    qDebug() << "MacOsOpenModel: open with args" << arguments;
    process->start(program,arguments);
    mProcessList.push_back(unique_ptr<QProcess>(process));
}

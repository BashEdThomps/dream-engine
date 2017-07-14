/*
 * main.cpp
 *
 * Created: 13 2017 by Ashley
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

#include "View/MainWindow.h"
#include "Controller/MainController.h"
#include <QApplication>
#include <QSurfaceFormat>

#include <DreamCore.h>

using Dream::Constants;

int main(int argc, char *argv[])
{
    Constants::dreamSetVerbose(true);
    QApplication a(argc, argv);
    QSurfaceFormat glFormat;
    glFormat.setVersion( 3, 2 );
    glFormat.setProfile( QSurfaceFormat::CoreProfile ); // Requires >=Qt-4.8.0
    glFormat.setSamples(4);
    QSurfaceFormat::setDefaultFormat(glFormat);
    MainWindow mainWindow;
    MainController mainController(&mainWindow);
    mainWindow.show();
    return a.exec();
}

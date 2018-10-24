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
#ifdef WIN32
        #include <Windows.h>
#endif
#include "Controller/MainWindowController.h"
#include "Controller/ApplicationController.h"
#include <QApplication>

#include <QSurfaceFormat>
#include <QTextStream>
#include <iostream>

using std::cerr;
using std::endl;

int main(int argc, char *argv[])
{
    spdlog::set_level(spdlog::level::off);
    spdlog::set_pattern("%H:%M:%S|%t|%n|%l|%v");

    QApplication a(argc, argv);

    QFile f(":qdarkstyle/style.qss");
    if (!f.exists())
    {
        cerr << "Unable to set stylesheet, file not found" << endl;
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

    QSurfaceFormat glFormat;
    glFormat.setVersion( 3, 2 );
    glFormat.setProfile( QSurfaceFormat::CoreProfile ); // Requires >=Qt-4.8.0
    glFormat.setSamples(4);
    QSurfaceFormat::setDefaultFormat(glFormat);
    ApplicationController mainController;
    return a.exec();
}

/*
 * QTDreamWindowComponent.cpp
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
#include "QTDreamWindowComponent.h"
#include <QDebug>
#include <QSurfaceFormat>
#include <QSurfaceFormat>

QTDreamWindowComponent::QTDreamWindowComponent(QWidget* parent)
    :  QOpenGLWidget(parent), IWindowComponent()
{
    mDreamEngine = nullptr;
    QSurfaceFormat glFormat;
    glFormat.setVersion(3,3);
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(glFormat);
    return;
}

QTDreamWindowComponent::~QTDreamWindowComponent()
{
    return;
}

void QTDreamWindowComponent::setDreamEngine(Dream::DreamEngine* engine)
{
    mDreamEngine = engine;
    return;
}

bool QTDreamWindowComponent::init()
{
    return true;
}

void QTDreamWindowComponent::updateComponent(Dream::Scene*)
{
    return;
}

void QTDreamWindowComponent::getCurrentDimensions()
{
    return;
}

void QTDreamWindowComponent::swapBuffers()
{
    return;
}

void QTDreamWindowComponent::initializeGL()
{
    return;
}

void QTDreamWindowComponent::resizeGL(int w, int h)
{
    setWidth(w);
    setHeight(h);
    return;
}

void QTDreamWindowComponent::paintGL()
{
    qDebug() << "QTDreamWindowComponent: paintGL() Called";
    if (mDreamEngine)
    {
        mDreamEngine->updateLogic();
        mDreamEngine->updateGraphics();
        mDreamEngine->updateCleanup();
    }
    else
    {
       qDebug() << "QTDreamWindowComponent: mDreamEngine == nullptr" ;
    }
    return;
}

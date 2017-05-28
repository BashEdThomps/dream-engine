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
#include <QOpenGLContext>

QTDreamWindowComponent::QTDreamWindowComponent(QWidget* parent)
    :  QOpenGLWidget(parent), IWindowComponent()
{
    mDreamEngine = nullptr;
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
    makeCurrent();
    QSurfaceFormat glFormat;
    glFormat.setDepthBufferSize(32);
    glFormat.setMajorVersion(3);
    glFormat.setMinorVersion(0);
    glFormat.setSamples(4);
    //glFormat.setProfile(QSurfaceFormat::CoreProfile);
    context()->setFormat(glFormat);
    doneCurrent();
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

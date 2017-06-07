#include "QOpenGLWindowComponent.h"
#include <QDebug>


QOpenGLWindowComponent::QOpenGLWindowComponent
( const QSurfaceFormat& format, QWidget* parent )
    : QOpenGLWidget(parent ), IWindowComponent ()
{
    setFormat(format);
    mProject = nullptr;
}

QOpenGLWindowComponent::~QOpenGLWindowComponent
()
{

}

void
QOpenGLWindowComponent::initializeGL
()
{
    // get context opengl-version
    qDebug() << "Widget OpenGl: " << format().majorVersion() << "." << format().minorVersion();
    qDebug() << "Context valid: " << context()->isValid();
    qDebug() << "Really used OpenGl: " << context()->format().majorVersion() << "." << context()->format().minorVersion();
    qDebug() << "OpenGl information: VENDOR:       " << (char*)glGetString(GL_VENDOR);
    qDebug() << "                    RENDERDER:    " << (char*)glGetString(GL_RENDERER);
    qDebug() << "                    VERSION:      " << (char*)glGetString(GL_VERSION);
    qDebug() << "                    GLSL VERSION: " << (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
}

void
QOpenGLWindowComponent::resizeGL
( int w, int h )
{
   setWidth(w);
   setHeight(h);
   mSizeHasChanged = true;
}

void
QOpenGLWindowComponent::paintGL
()
{
    if (mProject)
    {
        if (mProject->hasActiveScene())
        {
            if (mProject->getActiveScene()->getState() != DONE)
            {
                mProject->updateAll();
            }
            return;
        }
    }

    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool
QOpenGLWindowComponent::init
()
{
    return true;
}

void
QOpenGLWindowComponent::updateComponent
(Scene*)
{

}

void
QOpenGLWindowComponent::getCurrentDimensions
()
{

}

void
QOpenGLWindowComponent::swapBuffers
()
{

}

void
QOpenGLWindowComponent::setProject
(Project* engine)
{
    mProject = engine;
}

void
QOpenGLWindowComponent::cleanUp
(Scene* scene) {}

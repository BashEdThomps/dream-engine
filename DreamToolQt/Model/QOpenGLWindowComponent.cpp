#include "QOpenGLWindowComponent.h"
#include <QDebug>


QOpenGLWindowComponent::QOpenGLWindowComponent
( const QSurfaceFormat& format, QWidget* parent )
    : QOpenGLWidget(parent ), Dream::IWindowComponent ()
{
    setFormat(format);
    mDreamEngine = nullptr;
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
    if (mDreamEngine)
    {
        mDreamEngine->updateLogic();
        mDreamEngine->updateGraphics();
        mDreamEngine->updateCleanup();
    }
    return;
}

bool
QOpenGLWindowComponent::init
()
{
    return true;
}

void
QOpenGLWindowComponent::updateComponent
(Dream::Scene*)
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
QOpenGLWindowComponent::setDreamEngine
(Dream::DreamEngine* engine)
{
    mDreamEngine = engine;
    return;
}

void
QOpenGLWindowComponent::cleanUp
(Dream::Scene* scene)
{

}

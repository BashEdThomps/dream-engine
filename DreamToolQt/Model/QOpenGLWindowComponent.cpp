
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
    qDebug() << "QOpenGLWindowComponent: Widget OpenGl: " << format().majorVersion() << "." << format().minorVersion();
    qDebug() << "QOpenGLWindowComponent: Context valid: " << context()->isValid();
    qDebug() << "QOpenGLWindowComponent: Really used OpenGl: " << context()->format().majorVersion() << "." << context()->format().minorVersion();
    qDebug() << "QOpenGLWindowComponent: OpenGl information:";
    qDebug() << "QOpenGLWindowComponent: 	VENDOR:       " << (char*)glGetString(GL_VENDOR);
    qDebug() << "QOpenGLWindowComponent:    RENDERDER:    " << (char*)glGetString(GL_RENDERER);
    qDebug() << "QOpenGLWindowComponent:    VERSION:      " << (char*)glGetString(GL_VERSION);
    qDebug() << "QOpenGLWindowComponent:    GLSL VERSION: " << (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
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

    if (mGrid)
    {
        if (!mGrid->isInitialised())
        {
            mGrid->init();
        }
        mGrid->setViewMatrix(mProject->getRuntime()->getGraphicsComponent()->getViewMatrix());
        mGrid->setProjectionMatrix(mProject->getRuntime()->getGraphicsComponent()->getProjectionMatrix());
        mGrid->draw();
    }

    //glClearColor(0.0f,0.0f,0.0f,0.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
(Scene* scene)
{

}

void
QOpenGLWindowComponent::setGrid
(Grid* grid)
{
    mGrid = grid;
}


#include "QOpenGLWindowComponent.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPointF>


QOpenGLWindowComponent::QOpenGLWindowComponent
(const QSurfaceFormat& format, QWidget* parent)
    : QOpenGLWidget(parent),
      IWindowComponent(),
      mProjectHandle(nullptr),
      mGridHandle(nullptr),
      mGridEnabled(true),
      mSelectionHighlighterHandle(nullptr)
{
    setFormat(format);
}

QOpenGLWindowComponent::~QOpenGLWindowComponent
()
{
    mProjectHandle = nullptr;
    mGridHandle = nullptr;
    mSelectionHighlighterHandle = nullptr;
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
    if (mProjectHandle)
    {
        if (mProjectHandle->hasActiveScene())
        {
            if (mProjectHandle->getActiveScene()->getState() != DONE)
            {

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                mProjectHandle->updateLogic();

                if (mGridHandle && mGridEnabled)
                {
                    if (!mGridHandle->isInitialised())
                    {
                        mGridHandle->init();
                    }
                    mGridHandle->setViewMatrix(mProjectHandle->getRuntime()->getGraphicsComponent()->getViewMatrix());
                    mGridHandle->setProjectionMatrix(mProjectHandle->getRuntime()->getGraphicsComponent()->getProjectionMatrix());
                    mGridHandle->draw();
                }

                if (mSelectionHighlighterHandle)
                {
                    if (!mSelectionHighlighterHandle->isInitialised())
                    {
                        mSelectionHighlighterHandle->init();
                    }
                    mSelectionHighlighterHandle->setViewMatrix(mProjectHandle->getRuntime()->getGraphicsComponent()->getViewMatrix());
                    mSelectionHighlighterHandle->setProjectionMatrix(mProjectHandle->getRuntime()->getGraphicsComponent()->getProjectionMatrix());
                    mSelectionHighlighterHandle->draw();
                }

                mProjectHandle->updateGraphics();
                mProjectHandle->updateFlush();
            }
            return;
        }
    }
    // If no active scene, blank screen
    else
    {
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
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
    mProjectHandle = engine;
}

void
QOpenGLWindowComponent::cleanUp
(Scene*)
{

}

void
QOpenGLWindowComponent::setGrid
(Grid* grid)
{
    mGridHandle = grid;
}

void
QOpenGLWindowComponent::setSelectionHighlighter
(SelectionHighlighter* highlighter)
{
    mSelectionHighlighterHandle = highlighter;
}

void
QOpenGLWindowComponent::mouseMoveEvent
(QMouseEvent *event)
{
    QPointF pos = event->localPos();
    //qDebug() << "QOGLWC: Mouse event, local pos "
    //             << pos.x() << "," << pos.y();
    InputEvent dreamEvent;

    dreamEvent.setSource(INPUT_SOURCE_MOUSE);
    dreamEvent.setMouseEventType(MOUSE_MOTION);

    dreamEvent.setX(static_cast<int>(pos.x()-width()/2));
    dreamEvent.setY(static_cast<int>(pos.y()-height()/2));

    if (mProjectHandle)
    {
        mProjectHandle->getRuntime()->getLuaEngine()->addInputEvent(dreamEvent);
    }
}

void
QOpenGLWindowComponent::setGridEnabled
(bool enabled)
{
    mGridEnabled = enabled;
}

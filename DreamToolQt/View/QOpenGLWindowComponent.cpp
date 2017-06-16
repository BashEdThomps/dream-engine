
#include "QOpenGLWindowComponent.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPointF>


WindowInputState::WindowInputState
()
    : // Init list
     mouseLastX(0),
     mouseLastY(0),

     shiftPressed(false),

     wPressed(false),
     aPressed(false),
     sPressed(false),
     dPressed(false),

     upPressed(false),
     downPressed(false),
     leftPressed(false),
     rightPressed(false)
{

}

QOpenGLWindowComponent::QOpenGLWindowComponent
(const QSurfaceFormat& format, QWidget* parent)
    : QOpenGLWidget(parent),
      IWindowComponent(),
      mProjectHandle(nullptr),
      mGridHandle(nullptr),
      mSelectionHighlighterHandle(nullptr),
      mRelationshipTreeHandle(nullptr),
      mGridEnabled(true),
      mRelationshipTreeEnabled(true)
{
    setFormat(format);
}

QOpenGLWindowComponent::~QOpenGLWindowComponent
()
{
    mProjectHandle = nullptr;
    mGridHandle = nullptr;
    mSelectionHighlighterHandle = nullptr;
    mRelationshipTreeHandle = nullptr;
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
    updateInputState();
    if (mProjectHandle)
    {
        if (mProjectHandle->hasActiveScene())
        {
            if (mProjectHandle->getActiveScene()->getRuntime()->getState() != DONE)
            {

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                mProjectHandle->updateAll();

                if (mGridHandle)
                {
                    if(mGridEnabled)
                    {
                        if (!mGridHandle->isInitialised())
                        {
                            mGridHandle->init();

                            Constants::checkGLError("QOGLWC: After Grid Init");
                        }
                        mGridHandle->setViewMatrix(mProjectHandle->getRuntime()->getGraphicsComponent()->getViewMatrix());
                        mGridHandle->setProjectionMatrix(mProjectHandle->getRuntime()->getGraphicsComponent()->getProjectionMatrix());
                        mGridHandle->draw();
                    }
                    else
                    {
                        cout << "QOGLWC: Grid Disabled" << endl;
                        Constants::checkGLError("QOGLWC: After Grid Draw");
                    }
                }
                else
                {
                    cout << "QOGLWC: No Grid Handle" << endl;
                }


                if (mSelectionHighlighterHandle)
                {
                    if (!mSelectionHighlighterHandle->isInitialised())
                    {
                        mSelectionHighlighterHandle->init();
                Constants::checkGLError("QOGLWC: SelectionHighlighter after Init");
                    }
                    mSelectionHighlighterHandle->setViewMatrix
                            (
                                mProjectHandle->getRuntime()
                                ->getGraphicsComponent()
                                ->getViewMatrix()
                                );
                    mSelectionHighlighterHandle->setProjectionMatrix
                            (
                                mProjectHandle->getRuntime()
                                ->getGraphicsComponent()
                                ->getProjectionMatrix()
                                );
                    mSelectionHighlighterHandle->draw();
                Constants::checkGLError("QOGLWC: SelectionHighlighter after draw");
                }
                else if (Constants::DEBUG)
                {
                    cout << "QOGLWC: No SelectionHighlighter Handle" << endl;
                }



                if (mRelationshipTreeHandle && mRelationshipTreeEnabled)
                {
                    if (!mRelationshipTreeHandle->isInitialised())
                    {
                        mRelationshipTreeHandle->init();
                        Constants::checkGLError("QOGLWC: RelTree after init");
                    }
                    mRelationshipTreeHandle->setViewMatrix
                            (
                                mProjectHandle->getRuntime()
                                ->getGraphicsComponent()
                                ->getViewMatrix()
                                );
                    mRelationshipTreeHandle->setProjectionMatrix
                            (
                                mProjectHandle->getRuntime()
                                ->getGraphicsComponent()
                                ->getProjectionMatrix()
                                );
                    mRelationshipTreeHandle->draw();

                    Constants::checkGLError("QOGLWC: RelTree after draw");
                }


                /*
                 * mProjectHandle->updateGraphics();
                 * Constants::checkGLError("QOGLWC: after update gfx");
                 * mProjectHandle->updateFlush();
                 */
            }
        }
        else
        {
            cout << "QOpenGLWindowComponent: Cannot draw, no active scene" << endl;
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
    if (mProjectHandle)
    {
        QPointF pos = event->localPos();

        int x = static_cast<int>( pos.x() - ( getWidth()  / 2 ) );
        int y = static_cast<int>( pos.y() - ( getHeight() / 2 ) );

        int dX = x - mInputState.mouseLastX;
        int dY = y - mInputState.mouseLastY;

        mProjectHandle->getRuntime()
                      ->getCamera()
                      ->processMouseMovement(dX,dY,false);
        mInputState.mouseLastX = x;
        mInputState.mouseLastY = y;
    }

}

void
QOpenGLWindowComponent::updateInputState
()
{
    if (!mProjectHandle)
    {
        return;
    }

    Camera *camHandle = mProjectHandle->getRuntime()->getCamera();
    SceneObject *selected = mSelectionHighlighterHandle->getSelectedObject();
    Transform3D *transform = nullptr;

    float deltaTime = static_cast<float>
    (
        mProjectHandle->getRuntime()
                      ->getTime()
                      ->getTimeDelta()
    );

    if (mInputState.wPressed)
    {
        camHandle->processKeyboard(CAMERA_MOVEMENT_FORWARD,deltaTime);
    }

    if(mInputState.aPressed)
    {
        camHandle->processKeyboard(CAMERA_MOVEMENT_LEFT,deltaTime);
    }

    if (mInputState.sPressed)
    {
        camHandle->processKeyboard(CAMERA_MOVEMENT_BACKWARD,deltaTime);
    }

    if(mInputState.dPressed)
    {
        camHandle->processKeyboard(CAMERA_MOVEMENT_RIGHT,deltaTime);
    }

    if(mInputState.upPressed)
    {
        if (selected)
        {
            transform = selected->getTransform();
            if (transform)
            {
                transform->setTranslationZ(
                    transform->getTranslationZ() +
                    (
                        mInputState.shiftPressed ?
                        mGridHandle->getMajorSpacing() :
                        mGridHandle->getMinorSpacing()
                    )
                );
            }
        }
    }

    if(mInputState.downPressed)
    {
        if (selected)
        {
            transform = selected->getTransform();
            if (transform)
            {
                transform->setTranslationZ(
                    transform->getTranslationZ() -
                    (
                        mInputState.shiftPressed ?
                        mGridHandle->getMajorSpacing() :
                        mGridHandle->getMinorSpacing()
                    )
                );
            }
        }
    }

    if(mInputState.leftPressed)
    {
        if (selected)
        {
            transform = selected->getTransform();
            if (transform)
            {
                transform->setTranslationX(
                    transform->getTranslationX() -
                    (
                        mInputState.shiftPressed ?
                        mGridHandle->getMajorSpacing() :
                        mGridHandle->getMinorSpacing()
                    )
                );
            }
        }
    }

    if(mInputState.rightPressed)
    {
        if (selected)
        {
            transform = selected->getTransform();
            if (transform)
            {
                transform->setTranslationX(
                    transform->getTranslationX() +
                    (
                        mInputState.shiftPressed ?
                        mGridHandle->getMajorSpacing() :
                        mGridHandle->getMinorSpacing()
                    )
                );
            }
        }
    }
}

void
QOpenGLWindowComponent::keyPressEvent
(QKeyEvent *event)
{
    qDebug() << "QOpenGLWindowComponent: Pressed Key" << event->key();

    switch (event->key())
    {
        case Qt::Key_W:
            mInputState.wPressed = true;
            break;
        case Qt::Key_A:
            mInputState.aPressed = true;
            break;
        case Qt::Key_S:
            mInputState.sPressed = true;
            break;
        case Qt::Key_D:
            mInputState.dPressed = true;
            break;
        case Qt::Key_Shift:
            mInputState.shiftPressed = true;
            break;
        case Qt::Key_Up:
            mInputState.upPressed = true;
            break;
        case Qt::Key_Down:
            mInputState.downPressed = true;
            break;
        case Qt::Key_Left:
            mInputState.leftPressed = true;
            break;
        case Qt::Key_Right:
            mInputState.rightPressed = true;
            break;
    }
}

void
QOpenGLWindowComponent::setGridEnabled
(bool enabled)
{
    mGridEnabled = enabled;
}

void
QOpenGLWindowComponent::setRelationshipTree
(RelationshipTree* tree)
{
    mRelationshipTreeHandle = tree;
}

void
QOpenGLWindowComponent::keyReleaseEvent
(QKeyEvent* event)
{
    qDebug() << "QOpenGLWindowComponent: Released Key" << event->key();
    switch (event->key())
    {
        case Qt::Key_W:
            mInputState.wPressed = false;
            break;
        case Qt::Key_A:
            mInputState.aPressed = false;
            break;
        case Qt::Key_S:
            mInputState.sPressed = false;
            break;
        case Qt::Key_D:
            mInputState.dPressed = false;
            break;
        case Qt::Key_Shift:
            mInputState.shiftPressed = false;
            break;
        case Qt::Key_Up:
            mInputState.upPressed = false;
            break;
        case Qt::Key_Down:
            mInputState.downPressed = false;
            break;
        case Qt::Key_Left:
            mInputState.leftPressed = false;
            break;
        case Qt::Key_Right:
            mInputState.rightPressed = false;
            break;
    }
}

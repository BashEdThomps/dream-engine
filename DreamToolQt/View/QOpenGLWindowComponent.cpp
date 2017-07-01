
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
     altPressed(false),
     ctrlPressed(false),
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
      mProjectRuntimeHandle(nullptr),
      mGridHandle(nullptr),
      mSelectionHighlighterHandle(nullptr),
      mRelationshipTreeHandle(nullptr),
      mGridEnabled(true),
      mRelationshipTreeEnabled(true),
      mSelectionHighlighterEnabled(true),
      mPaintInProgress(false)
{
    setFormat(format);
}

QOpenGLWindowComponent::~QOpenGLWindowComponent
()
{
    mProjectRuntimeHandle = nullptr;
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
    if (mPaintInProgress)
    {
        cerr << "QOGLWC: Attempted to paint before previous paintGL call has finished" << endl;
        return;
    }

    mPaintInProgress = true;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    if (mProjectRuntimeHandle)
    {
        if (mProjectRuntimeHandle->hasActiveSceneRuntime())
        {
            updateInputState();
            SceneRuntime *sRuntime = mProjectRuntimeHandle->getActiveSceneRuntimeHandle();


            if (sRuntime->getState() != SCENE_STATE_STOPPED)
            {
                mProjectRuntimeHandle->updateAll();

                glm::mat4 viewMatrix = mProjectRuntimeHandle->getGraphicsComponentHandle()->getViewMatrix();
                glm::mat4 projectionMatrix = mProjectRuntimeHandle->getGraphicsComponentHandle()->getProjectionMatrix();

                if (mGridHandle)
                {
                    if(mGridEnabled)
                    {
                        if (!mGridHandle->isInitialised())
                        {
                            mGridHandle->init();

                            Constants::checkGLError("QOGLWC: After Grid Init");
                        }
                        mGridHandle->setViewMatrix(viewMatrix);
                        mGridHandle->setProjectionMatrix(projectionMatrix);
                        mGridHandle->draw();
                    }
                    else
                    {
                        cout << "QOGLWC: Grid Disabled" << endl;
                        Constants::checkGLError("QOGLWC: After Grid Draw");
                    }
                }

                if (mSelectionHighlighterHandle)
                {
                    if (!mSelectionHighlighterHandle->isInitialised())
                    {
                        mSelectionHighlighterHandle->init();
                        Constants::checkGLError("QOGLWC: SelectionHighlighter after Init");
                    }
                    mSelectionHighlighterHandle->setViewMatrix(viewMatrix);
                    mSelectionHighlighterHandle->setProjectionMatrix(projectionMatrix);
                    mSelectionHighlighterHandle->draw();
                    Constants::checkGLError("QOGLWC: SelectionHighlighter after draw");
                }

                if (mRelationshipTreeHandle && mRelationshipTreeEnabled)
                {
                    if (!mRelationshipTreeHandle->isInitialised())
                    {
                        mRelationshipTreeHandle->init();
                        Constants::checkGLError("QOGLWC: RelTree after init");
                    }
                    mRelationshipTreeHandle->setViewMatrix(viewMatrix);
                    mRelationshipTreeHandle->setProjectionMatrix(projectionMatrix);
                    mRelationshipTreeHandle->draw();

                    Constants::checkGLError("QOGLWC: RelTree after draw");
                }
            }
            else
            {
                mProjectRuntimeHandle = nullptr;
            }
        }
    }
    // If no active scene, blank screen
    else
    {
        glClearColor(0.0f,0.0f,0.0f,0.0f);
    }

    glDisable(GL_DEPTH_TEST);

    mPaintInProgress = false;
}

bool
QOpenGLWindowComponent::init
()
{
    return true;
}

void
QOpenGLWindowComponent::updateComponent
(SceneRuntime*)
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
QOpenGLWindowComponent::setProjectRuntimeHandle
(ProjectRuntime* engine)
{
    mProjectRuntimeHandle = engine;
}

void
QOpenGLWindowComponent::setGridHandle
(Grid* grid)
{
    mGridHandle = grid;
}

void
QOpenGLWindowComponent::setSelectionHighlighterHandle
(SelectionHighlighter* highlighter)
{
    mSelectionHighlighterHandle = highlighter;
}

void
QOpenGLWindowComponent::setRelationshipTreeHandle
(RelationshipTree* tree)
{
    mRelationshipTreeHandle = tree;
}

void
QOpenGLWindowComponent::mouseMoveEvent
(QMouseEvent *event)
{
    if (mProjectRuntimeHandle)
    {
        QPointF pos = event->localPos();

        int x = static_cast<int>( pos.x() - ( getWidth()  / 2 ) );
        int y = static_cast<int>( pos.y() - ( getHeight() / 2 ) );

        int dX = x - mInputState.mouseLastX;
        int dY = y - mInputState.mouseLastY;

        int threshold = 150;
        if (
            dX < threshold && dX > -threshold
            &&
            dY < threshold && dY > -threshold
        )
        {
            mProjectRuntimeHandle->getCameraHandle()->processMouseMovement(dX,dY,true);
        }
        mInputState.mouseLastX = x;
        mInputState.mouseLastY = y;
    }

}

void
QOpenGLWindowComponent::updateInputState
()
{
    if (!mProjectRuntimeHandle)
    {
        return;
    }

    Camera *camHandle = mProjectRuntimeHandle->getCameraHandle();
    SceneObjectRuntime *selected = nullptr;
    if (mSelectionHighlighterHandle)
    {
        selected = mSelectionHighlighterHandle->getSelectedObject();
    }

    float deltaTime = static_cast<float>
    (
        mProjectRuntimeHandle->getTimeHandle()->getTimeDelta()
    );

    if (mInputState.wPressed)
    {
        camHandle->processKeyboard(Constants::CAMERA_MOVEMENT_FORWARD,deltaTime);
    }

    if(mInputState.aPressed)
    {
        camHandle->processKeyboard(Constants::CAMERA_MOVEMENT_LEFT,deltaTime);
    }

    if (mInputState.sPressed)
    {
        camHandle->processKeyboard(Constants::CAMERA_MOVEMENT_BACKWARD,deltaTime);
    }

    if(mInputState.dPressed)
    {
        camHandle->processKeyboard(Constants::CAMERA_MOVEMENT_RIGHT,deltaTime);
    }


    if (selected)
    {
        Transform3D transform = selected->getTransform();
        float moveAmount = mInputState.shiftPressed ?
                               mGridHandle->getMajorSpacing() :
                               mGridHandle->getMinorSpacing();
        if(mInputState.upPressed)
        {
            qDebug() << "QOGLWC: Moving Selected up";
            if (mInputState.altPressed)
            {
                transform.translateByY(moveAmount);
            }
            else
            {
                transform.translateByZ(moveAmount);
            }
        }

        if(mInputState.downPressed)
        {
            qDebug() << "QOGLWC: Moving Selected down";
            if (mInputState.altPressed)
            {
                transform.translateByY(-moveAmount);
            }
            else
            {
                transform.translateByZ(-moveAmount);
            }
        }

        if(mInputState.leftPressed)
        {
            qDebug() << "QOGLWC: Moving Selected left";
            transform.translateByX(-moveAmount);
        }

        if(mInputState.rightPressed)
        {
            qDebug() << "QOGLWC: Moving Selected right";
            transform.translateByX(moveAmount);
        }
        selected->setTransform(transform);
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
        case Qt::Key_Alt:
            mInputState.altPressed = true;
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
QOpenGLWindowComponent::setSelectionHighlighterEnabled
(bool enabled)
{
    mSelectionHighlighterEnabled = enabled;
}

void
QOpenGLWindowComponent::setRelationshipTreeEnabled
(bool enabled)
{
    mRelationshipTreeEnabled = enabled;
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
        case Qt::Key_Alt:
            mInputState.altPressed = false;
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

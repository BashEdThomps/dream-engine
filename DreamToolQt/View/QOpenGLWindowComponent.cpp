
#include "QOpenGLWindowComponent.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPointF>
#include <QPainter>
#include <spdlog/spdlog.h>
#include <DreamCore.h>
#include <QChar>

WindowInputState::WindowInputState
()
    : // Init list
      mouseLastX(0),
      mouseLastY(0),
      mouseWheelLastX(0),
      mouseWheelLastY(0),
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
(QWidget* parent)
    : QOpenGLWidget(parent),
      IWindowComponent(false),
      mControlScene(false),
      mProjectRuntimeHandle(nullptr),
      mGridHandle(nullptr),
      mSelectionHighlighterHandle(nullptr),
      mRelationshipTreeHandle(nullptr),
      mPathPointViewerHandle(nullptr),
      mPaintInProgress(false),
      mGridEnabled(true),
      mRelationshipTreeEnabled(true),
      mSelectionHighlighterEnabled(true),
      mMaxFrameTimeValues(100)

{
    auto log = spdlog::get("QOpenGLWindowComponent");
    if (log==nullptr)
    {
        log = spdlog::stdout_color_mt("QOpenGLWindowComponent");
    }
    log->info("Constructing");
    //setFormat(format);
}

QOpenGLWindowComponent::~QOpenGLWindowComponent
()
{
    auto log = spdlog::get("QOpenGLWindowComponent");
    log->info("Destructing");
    mProjectRuntimeHandle = nullptr;
    mGridHandle = nullptr;
    mSelectionHighlighterHandle = nullptr;
    mRelationshipTreeHandle = nullptr;
    mPathPointViewerHandle = nullptr;
}

void
QOpenGLWindowComponent::initializeGL
()
{
    auto log = spdlog::get("QOpenGLWindowComponent");
    // get context opengl-version
    log->info("OpenGL Version: {}.{}", format().majorVersion(), format().minorVersion());
    log->info("Context valid: {}",  context()->isValid());
    log->info("Really used OpenGl: {}.{}", context()->format().majorVersion(), context()->format().minorVersion());
    log->info("OpenGl information:");
    log->info("VENDOR:       {}", static_cast<const unsigned char*>(glGetString(GL_VENDOR)));
    log->info("RENDERDER:    {}", static_cast<const unsigned char*>(glGetString(GL_RENDERER)));
    log->info("VERSION:      {}", static_cast<const unsigned char*>(glGetString(GL_VERSION)));
    log->info("GLSL VERSION: {}", static_cast<const unsigned char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
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
    auto log = spdlog::get("QOpenGLWindowComponent");
    if (mPaintInProgress)
    {

        log->trace("PaintGL all ready in progress");
        return;
    }

    log->trace("PaintGL");

    if (mProjectRuntimeHandle)
    {
        if (mProjectRuntimeHandle->hasActiveSceneRuntime())
        {
            mPaintInProgress = true;
            updateInputState();
            SceneRuntime *sRuntime = mProjectRuntimeHandle->getActiveSceneRuntimeHandle();

            if (sRuntime->getState() != SCENE_STATE_STOPPED)
            {
                if (mProjectRuntimeHandle->getParallel())
                {
                    // Wait for an update to trigger
                    while(!mProjectRuntimeHandle->updateLogic())
                    {
                        std::this_thread::yield();
                    }

                    // Wait for update threads to finish
                    while (!mProjectRuntimeHandle->allThreadsHaveUpdated())
                    {
                        std::this_thread::yield();
                    }
                }
                else
                {
                    mProjectRuntimeHandle->updateLogic();
                }

                mProjectRuntimeHandle->collectGarbage();

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glEnable(GL_DEPTH_TEST);

                mProjectRuntimeHandle->updateGraphics();

                glm::mat4 viewMatrix = mProjectRuntimeHandle->getGraphicsComponentHandle()->getViewMatrix();
                glm::mat4 projectionMatrix = mProjectRuntimeHandle->getGraphicsComponentHandle()->getProjectionMatrix();

                // glDisable(GL_DEPTH_TEST);

                log->trace("Drawing Grid");
                if (mGridHandle)
                {
                    if(mGridEnabled)
                    {
                        if (!mGridHandle->isInitialised())
                        {
                            mGridHandle->init();

                            Constants::checkGLError("After Grid Init");
                        }
                        mGridHandle->setViewMatrix(viewMatrix);
                        mGridHandle->setProjectionMatrix(projectionMatrix);
                        mGridHandle->draw();
                    }
                    else
                    {
                        log->trace("Grid Disabled");
                        Constants::checkGLError("After Grid Draw");
                    }
                }

                log->trace("Drawing Selection Highlighter");
                if (mSelectionHighlighterHandle)
                {
                    if (mSelectionHighlighterEnabled)
                    {
                        if (!mSelectionHighlighterHandle->isInitialised())
                        {
                            mSelectionHighlighterHandle->init();
                            Constants::checkGLError("SelectionHighlighter after Init");
                        }

                        mSelectionHighlighterHandle->setViewMatrix(viewMatrix);
                        mSelectionHighlighterHandle->setProjectionMatrix(projectionMatrix);
                        mSelectionHighlighterHandle->draw();
                        Constants::checkGLError("SelectionHighlighter after draw");
                    }
                }

                log->trace("Drawing Relationship Tree");
                if (mRelationshipTreeHandle)
                {
                    if (mRelationshipTreeEnabled)
                    {
                        if (!mRelationshipTreeHandle->isInitialised())
                        {
                            mRelationshipTreeHandle->init();
                            Constants::checkGLError("RelTree after init");
                        }
                        mRelationshipTreeHandle->setViewMatrix(viewMatrix);
                        mRelationshipTreeHandle->setProjectionMatrix(projectionMatrix);
                        mRelationshipTreeHandle->draw();
                        Constants::checkGLError("RelTree after draw");
                    }
                }

                log->trace("Drawing Path List Nodes");
                if (mPathPointViewerHandle)
                {
                    if (!mPathPointViewerHandle->isInitialised())
                    {
                        mPathPointViewerHandle->init();
                        Constants::checkGLError("AnimViewer after init");
                    }
                    mPathPointViewerHandle->setViewMatrix(viewMatrix);
                    mPathPointViewerHandle->setProjectionMatrix(projectionMatrix);
                    mPathPointViewerHandle->draw();
                    Constants::checkGLError("AnimViewer after draw");
                }
                drawStats();
            }
            else
            {
                mProjectRuntimeHandle = nullptr;
            }
            mPaintInProgress = false;
        }
    }
    // If no active scene, blank screen
    else
    {
        glClearColor(0.0f,0.0f,0.0f,0.0f);
    }
}

void
QOpenGLWindowComponent::drawStats()
{
    auto project = mActiveSceneRuntimeHandle->getProjectRuntimeHandle();
    double frame = project->getTimeHandle()->getFrameTimeDelta();
    mFrameTimes.push_back(frame);

    if (mFrameTimes.size() > mMaxFrameTimeValues)
    {
        mFrameTimes.erase(begin(mFrameTimes));
    }

    long long path = project->getPathComponentHandle()->getUpdateTime();
    long long pathYield = project->getPathComponentHandle()->getYieldedTime();

    long long audio = project->getAudioComponentHandle()->getUpdateTime();
    long long  audioYield = project->getAudioComponentHandle()->getYieldedTime() ;

    long long graphics = project->getGraphicsComponentHandle()->getUpdateTime();
    long long graphicsYield = project->getGraphicsComponentHandle()->getYieldedTime();

    long long lua = project->getLuaComponentHandle()->getUpdateTime();
    long long luaYield = project->getLuaComponentHandle()->getYieldedTime();

    long long physics = project->getPhysicsComponentHandle()->getUpdateTime();
    long long physicsYield = project->getPhysicsComponentHandle()->getYieldedTime();

    bool madDetail = false;;
    QPainter painter(this);

    int topLeftX, topLeftY;
    topLeftX = 10;
    topLeftY = 10;

    QRectF rect;
    if (madDetail)
    {
        rect = QRect(0,0,getWidth(),getHeight()/10);
    }
    else
    {
        rect = QRect(0,0,getWidth(), 36);
    }
    QBrush brush(QColor(0,0,0,96));

    painter.fillRect(rect,brush);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 16));
    QString text;

    if (madDetail)
    {
        text =
                "Frame:     %1s     %12 FPS\n"
                "Path: %2µs  / %3µs | Audio: %4µs / %5µs\n"
                "Graphics:  %6µs  / %7µs | Lua:   %8µs / %9µs\n"
                "Physics:   %10µs / %11µs";
        text = text
                .arg(frame)
                .arg(path, 9, 10, QChar('0'))
                .arg(pathYield, 9, 10, QChar('0'))
                .arg(audio, 9, 10, QChar('0'))
                .arg(audioYield, 9, 10, QChar('0'))
                .arg(graphics, 9, 10, QChar('0'))
                .arg(graphicsYield, 9, 10, QChar('0'))
                .arg(lua, 9, 10, QChar('0'))
                .arg(luaYield, 9, 10, QChar('0'))
                .arg(physics, 9, 10, QChar('0'))
                .arg(physicsYield, 9,10, QChar('0'))
                .arg(1.0/averageFrameTime());
    }
    else
    {
        text = QString("FPS %1").arg(1.0/averageFrameTime());
    }
    painter.drawText(topLeftX, topLeftY,getWidth(), getHeight(), Qt::AlignLeft,text);
    painter.end();
}

bool QOpenGLWindowComponent::getControlScene() const
{
    return mControlScene;
}

void QOpenGLWindowComponent::setControlScene(bool controlScene)
{
    auto log = spdlog::get("QOpenGLWindowComponent");
    log->info("Scene Control Enabled {}",controlScene);
    mControlScene = controlScene;
}

double QOpenGLWindowComponent::averageFrameTime()
{
    double total = 0.0;
    for (double ft : mFrameTimes)
    {
        total += ft;
    }
    return total / mFrameTimes.size();
}

bool
QOpenGLWindowComponent::init
()
{
    return true;
}

void QOpenGLWindowComponent::updateComponent(){}

void QOpenGLWindowComponent::getCurrentDimensions(){}

void QOpenGLWindowComponent::swapBuffers(){}

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

void QOpenGLWindowComponent::setPathPointViewerHandle(PathPointViewer* handle)
{
   mPathPointViewerHandle = handle;
}

void
QOpenGLWindowComponent::wheelEvent
(QWheelEvent* event)
{
    if (mControlScene)
    {
        auto log = spdlog::get("QOpenGLWindowComponent");
        log->trace("WheelEvent");
        if (mProjectRuntimeHandle)
        {
            QPoint pos = event->pixelDelta();
            int x = static_cast<int>( pos.x() );
            int y = static_cast<int>( pos.y() );
            mProjectRuntimeHandle->getCameraHandle()->processMouseMovement(x,-y,true);
        }
    }
}

void
QOpenGLWindowComponent::mouseMoveEvent
(QMouseEvent *event)
{
    if (mControlScene)
    {
        auto log = spdlog::get("QOpenGLWindowComponent");
        log->trace("MouseMoveEvent");
        auto pos = event->localPos();
        mMouseX = pos.x();
        mMouseY = pos.y();
    }
}

void
QOpenGLWindowComponent::moveSelectedSceneObject
(SceneObjectRuntime* selected)
{
    if (mControlScene)
    {
        auto log = spdlog::get("QOpenGLWindowComponent");
        Transform3D transform = selected->getTransform();
        float moveAmount = mInputState.shiftPressed ?
                    mGridHandle->getMajorSpacing() :
                    mGridHandle->getMinorSpacing();

        if(mInputState.upPressed)
        {
            log->trace("Moving Selected up");
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
            log->trace("Moving Selected down");
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
            log->trace("Moving Selected left");
            transform.translateByX(-moveAmount);
        }

        if(mInputState.rightPressed)
        {
            log->trace("Moving Selected right");
            transform.translateByX(moveAmount);
        }

        selected->setTransform(transform);
    }
}

void
QOpenGLWindowComponent::updateInputState
()
{
    if (mControlScene)
    {
        auto log = spdlog::get("QOpenGLWindowComponent");
        if (!mProjectRuntimeHandle)
        {
            return;
        }

        SceneObjectRuntime *selected = nullptr;

        if (mSelectionHighlighterHandle)
        {
            selected = mSelectionHighlighterHandle->getSelectedObject();
        }

        if (selected)
        {
            moveSelectedSceneObject(selected);
        }

        moveCamera();
    }
}

void
QOpenGLWindowComponent::moveCamera
()
{

    auto log = spdlog::get("QOpenGLWindowComponent");
    log->trace("MoveCamera");
    Camera *camHandle = mProjectRuntimeHandle->getCameraHandle();
    float deltaTime = static_cast<float>(mProjectRuntimeHandle->getTimeHandle()->getFrameTimeDelta());

    if (mInputState.wPressed)
    {
        camHandle->processKeyboard
        (
            mInputState.altPressed ? Constants::CAMERA_MOVEMENT_UP : Constants::CAMERA_MOVEMENT_FORWARD,
            deltaTime
        );
    }

    if(mInputState.aPressed)
    {
        camHandle->processKeyboard(Constants::CAMERA_MOVEMENT_LEFT,deltaTime);
    }

    if (mInputState.sPressed)
    {
        camHandle->processKeyboard
        (
            mInputState.altPressed ? Constants::CAMERA_MOVEMENT_DOWN : Constants::CAMERA_MOVEMENT_BACKWARD,
            deltaTime
        );
    }

    if(mInputState.dPressed)
    {
        camHandle->processKeyboard(Constants::CAMERA_MOVEMENT_RIGHT,deltaTime);
    }
}

void
QOpenGLWindowComponent::keyPressEvent
(QKeyEvent *event)
{
    if (mControlScene)
    {
        auto log = spdlog::get("QOpenGLWindowComponent");
        log->trace("Pressed Key {}", event->key());

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
    if (mControlScene)
    {
        auto log = spdlog::get("QOpenGLWindowComponent");
        log->trace("Released Key {}", event->key());
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
}

void QOpenGLWindowComponent::clearProjectRuntimeHandle()
{
    mProjectRuntimeHandle = nullptr;
}

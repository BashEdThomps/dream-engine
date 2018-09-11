
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
      IWindowComponent(),
      mControlScene(false),
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

    auto projectRuntimeHandle = mProjectRuntimeHandle.lock();

    if (projectRuntimeHandle != nullptr)
    {
        if (projectRuntimeHandle->hasActiveSceneRuntime())
        {
            mPaintInProgress = true;
            updateInputState();
            weak_ptr<SceneRuntime> sRuntimeWeak = projectRuntimeHandle->getActiveSceneRuntime();
            auto sRuntime = sRuntimeWeak.lock();

            if (sRuntime == nullptr)
            {
                return;
            }

            if (sRuntime->getState() != SCENE_STATE_STOPPED)
            {

                projectRuntimeHandle->updateLogic();

                projectRuntimeHandle->collectGarbage();

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glEnable(GL_DEPTH_TEST);

                projectRuntimeHandle->updateGraphics();

                weak_ptr<GraphicsComponent> gfxRuntimeWeak = projectRuntimeHandle->getGraphicsComponent();
                auto gfxRuntime = gfxRuntimeWeak.lock();

                if (gfxRuntime == nullptr)
                {
                    return;
                }

                glm::mat4 viewMatrix = gfxRuntime->getViewMatrix();
                glm::mat4 projectionMatrix = gfxRuntime->getProjectionMatrix();

                // glDisable(GL_DEPTH_TEST);

                log->trace("Drawing Grid");
                if (mGridHandle)
                {
                    if(mGridEnabled)
                    {
                        if (!mGridHandle->isInitialised())
                        {
                            mGridHandle->init();

                            checkGLError();
                        }
                        mGridHandle->setViewMatrix(viewMatrix);
                        mGridHandle->setProjectionMatrix(projectionMatrix);
                        mGridHandle->draw();
                    }
                    else
                    {
                        log->trace("Grid Disabled");
                        checkGLError();
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
                            checkGLError();
                        }

                        mSelectionHighlighterHandle->setViewMatrix(viewMatrix);
                        mSelectionHighlighterHandle->setProjectionMatrix(projectionMatrix);
                        mSelectionHighlighterHandle->draw();
                        checkGLError();
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
                            checkGLError();
                        }
                        mRelationshipTreeHandle->setViewMatrix(viewMatrix);
                        mRelationshipTreeHandle->setProjectionMatrix(projectionMatrix);
                        mRelationshipTreeHandle->draw();
                        checkGLError();
                    }
                }

                log->trace("Drawing Path List Nodes");
                if (mPathPointViewerHandle)
                {
                    if (!mPathPointViewerHandle->isInitialised())
                    {
                        mPathPointViewerHandle->init();
                        checkGLError();
                    }
                    mPathPointViewerHandle->setViewMatrix(viewMatrix);
                    mPathPointViewerHandle->setProjectionMatrix(projectionMatrix);
                    mPathPointViewerHandle->draw();
                    checkGLError();
                }
                drawStats();
            }
            mPaintInProgress = false;
        }
    }
    // If no active scene, blank screen
    else
    {
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void
QOpenGLWindowComponent::drawStats()
{
    auto asr = mActiveSceneRuntime.lock();
    if (asr == nullptr) return;

    auto project = asr->getProjectRuntime();
    if (project == nullptr) return;

    auto time = project->getTime().lock();
    if (time == nullptr) return;

    double frame = time->getFrameTimeDelta();
    mFrameTimes.push_back(frame);

    if (mFrameTimes.size() > mMaxFrameTimeValues)
    {
        mFrameTimes.erase(begin(mFrameTimes));
    }

    long long path      = 0;
    long long pathYield = 0;

    auto pathComp = project->getPathComponent().lock();
    if (pathComp != nullptr)
    {
        path = pathComp->getUpdateTime();
        pathYield = pathComp->getYieldedTime();
    }

    long long audio = 0;
    long long  audioYield = 0;

    auto audioComp = project->getAudioComponent().lock();
    if (audioComp != nullptr)
    {
        audio = audioComp->getUpdateTime();
        audioYield = audioComp->getYieldedTime();
    }

    long long graphics = 0;
    long long graphicsYield = 0;
    auto gfxComp = project->getGraphicsComponent().lock();
    if (gfxComp != nullptr)
    {
        graphics = gfxComp->getUpdateTime();
        graphicsYield = gfxComp->getYieldedTime();
    }


    long long lua = 0;
    long long luaYield = 0;

    auto luaComp = project->getScriptComponent().lock();
    if (luaComp != nullptr)
    {
        lua = luaComp->getUpdateTime();
        luaYield = luaComp->getYieldedTime();
    }

    long long physics = 0;
    long long physicsYield = 0;

    auto physicsComp = project->getPhysicsComponent().lock();
    if (physicsComp != nullptr)
    {
        physics = physicsComp->getUpdateTime();
        physicsYield = physicsComp->getYieldedTime();
    }

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

    QFont font("Arial", 16);
    painter.setPen(Qt::white);
    painter.setFont(font);

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
QOpenGLWindowComponent::setProjectRuntime
(weak_ptr<ProjectRuntime> engine)
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
        auto projectRuntime = mProjectRuntimeHandle.lock();
        if (projectRuntime)
        {
            QPoint pos = event->pixelDelta();
            int x = static_cast<int>( pos.x() );
            int y = static_cast<int>( pos.y() );
            weak_ptr<Camera> camWeak = projectRuntime->getCamera();
            auto cam = camWeak.lock();
            if (cam != nullptr)
            {
                cam->processMouseMovement(x,-y,true);
            }
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
(weak_ptr<SceneObjectRuntime> selectedPtr)
{
    if (mControlScene)
    {
        auto log = spdlog::get("QOpenGLWindowComponent");

        auto selected = selectedPtr.lock();
        if (selected == nullptr)
        {
            return;
        }

        weak_ptr<Transform3D> transformWeak = selected->getTransform();
        auto transform = transformWeak.lock();

        if (transform == nullptr)
        {
            return;
        }

        float moveAmount = mInputState.shiftPressed ?
                    mGridHandle->getMajorSpacing() :
                    mGridHandle->getMinorSpacing();

        if(mInputState.upPressed)
        {
            log->trace("Moving Selected up");
            if (mInputState.altPressed)
            {
                transform->translateByY(moveAmount);
            }
            else
            {
                transform->translateByZ(moveAmount);
            }
        }

        if(mInputState.downPressed)
        {
            log->trace("Moving Selected down");
            if (mInputState.altPressed)
            {
                transform->translateByY(-moveAmount);
            }
            else
            {
                transform->translateByZ(-moveAmount);
            }
        }

        if(mInputState.leftPressed)
        {
            log->trace("Moving Selected left");
            transform->translateByX(-moveAmount);
        }

        if(mInputState.rightPressed)
        {
            log->trace("Moving Selected right");
            transform->translateByX(moveAmount);
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
        auto projectRuntimeHandle = mProjectRuntimeHandle.lock();
        if (!projectRuntimeHandle)
        {
            return;
        }

        weak_ptr<SceneObjectRuntime> selectedPtr;

        if (mSelectionHighlighterHandle)
        {
            selectedPtr = mSelectionHighlighterHandle->getSelectedObject();
            moveSelectedSceneObject(selectedPtr);
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

    auto projRuntime = mProjectRuntimeHandle.lock();

    if (projRuntime == nullptr )
    {
        return;
    }

    weak_ptr<Camera> camPtr = projRuntime->getCamera();
    weak_ptr<Time> timePtr =  projRuntime->getTime();

    auto camHandle = camPtr.lock();
    auto timeHandle = timePtr.lock();

    float deltaTime = 0.0;
    if (timeHandle != nullptr)
    {
        deltaTime = static_cast<float>(timeHandle->getFrameTimeDelta());
    }

    if (camHandle == nullptr)
    {
        return;
    }

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

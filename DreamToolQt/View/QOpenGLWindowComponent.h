#pragma once

#include <DreamCore.h>
#include "../View/GLView/Grid.h"
#include "../View/GLView/SelectionHighlighter.h"
#include "../View/GLView/RelationshipTree.h"
#include "../View/GLView/PathPointViewer.h"


#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <QOpenGLContext>

using namespace Dream;
using std::shared_ptr;

class WindowInputState
{
public:
    WindowInputState();

    int  mouseLastX;
    int  mouseLastY;

    int mouseWheelLastX;
    int mouseWheelLastY;

    bool shiftPressed;
    bool altPressed;
    bool ctrlPressed;

    bool wPressed;
    bool aPressed;
    bool sPressed;
    bool dPressed;

    bool upPressed;
    bool downPressed;
    bool leftPressed;
    bool rightPressed;
};

class QOpenGLWindowComponent : public QOpenGLWidget, public IWindowComponent
{
    Q_OBJECT
public:
    QOpenGLWindowComponent(QWidget* parent = nullptr );
    ~QOpenGLWindowComponent() override;

    bool init() override;
    void updateComponent() override;
    void getCurrentDimensions() override;
    void swapBuffers() override;
    void setProjectRuntime(shared_ptr<ProjectRuntime> prHandle);
    void setGridHandle(Grid* grid);
    void setSelectionHighlighterHandle(SelectionHighlighter* highlighter);
    void setRelationshipTreeHandle(RelationshipTree* tree);
    void setPathPointViewerHandle(PathPointViewer* handle);

    void setGridEnabled(bool enabled);
    void setSelectionHighlighterEnabled(bool enabled);
    void setRelationshipTreeEnabled(bool enabled);

    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    bool getControlScene() const;
    void setControlScene(bool controlScene);

protected:
    void initializeGL() override;
    void resizeGL( int w, int h ) override;
    void paintGL() override;
    void updateInputState();
    void wheelEvent(QWheelEvent* event) override;
    void drawStats();

private:
    bool mControlScene;
    shared_ptr<ProjectRuntime> mProjectRuntimeHandle;
    Grid *mGridHandle;
    SelectionHighlighter *mSelectionHighlighterHandle;
    RelationshipTree *mRelationshipTreeHandle;
    PathPointViewer* mPathPointViewerHandle;
    volatile bool mPaintInProgress;

    bool mGridEnabled;
    bool mRelationshipTreeEnabled;
    bool mSelectionHighlighterEnabled;

    WindowInputState mInputState;
    vector<double> mFrameTimes;
    size_t mMaxFrameTimeValues;
    double averageFrameTime();
    void moveSelectedSceneObject(shared_ptr<SceneObjectRuntime> selected);
    void moveCamera();
};

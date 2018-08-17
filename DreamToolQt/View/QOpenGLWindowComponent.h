#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <DreamCore.h>
#include "../View/GLView/Grid.h"
#include "../View/GLView/SelectionHighlighter.h"
#include "../View/GLView/RelationshipTree.h"


#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <QOpenGLContext>

using namespace Dream;

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
    void setProjectRuntimeHandle(ProjectRuntime* prHandle);
    void setGridHandle(Grid* grid);
    void setSelectionHighlighterHandle(SelectionHighlighter* highlighter);
    void setRelationshipTreeHandle(RelationshipTree* tree);

    void setGridEnabled(bool enabled);
    void setSelectionHighlighterEnabled(bool enabled);
    void setRelationshipTreeEnabled(bool enabled);

    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void clearProjectRuntimeHandle();

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
    ProjectRuntime* mProjectRuntimeHandle;
    Grid *mGridHandle;
    SelectionHighlighter *mSelectionHighlighterHandle;
    RelationshipTree *mRelationshipTreeHandle;
    volatile bool mPaintInProgress;

    bool mGridEnabled;
    bool mRelationshipTreeEnabled;
    bool mSelectionHighlighterEnabled;

    WindowInputState mInputState;
    vector<double> mFrameTimes;
    int mMaxFrameTimeValues;
    double averageFrameTime();
    void moveSelectedSceneObject(SceneObjectRuntime* selected);
    void moveCamera();
};

#endif // GLWIDGET_H

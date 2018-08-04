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
    void updateComponent(SceneRuntime*) override;
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

protected:
    void initializeGL() override;
    void resizeGL( int w, int h ) override;
    void paintGL() override;
    void updateInputState();
    void wheelEvent(QWheelEvent* event) override;

private:
    ProjectRuntime* mProjectRuntimeHandle;
    Grid *mGridHandle;
    SelectionHighlighter *mSelectionHighlighterHandle;
    RelationshipTree *mRelationshipTreeHandle;

    bool mGridEnabled;
    bool mRelationshipTreeEnabled;
    bool mSelectionHighlighterEnabled;

    WindowInputState mInputState;
    bool mPaintInProgress;
};

#endif // GLWIDGET_H

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

    bool shiftPressed;

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
    QOpenGLWindowComponent(const QSurfaceFormat& format, QWidget* parent = 0 );
    ~QOpenGLWindowComponent();

    bool init() override;
    void updateComponent(SceneRuntime*) override;
    void getCurrentDimensions() override;
    void swapBuffers() override;
    void cleanUp(SceneRuntime*) override;
    void setProject(Project* engine);
    void setGrid(Grid* grid);
    void setGridEnabled(bool enabled);
    void setSelectionHighlighter(SelectionHighlighter* highlighter);
    void setRelationshipTree(RelationshipTree* tree);

    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

protected:
    void initializeGL() override;
    void resizeGL( int w, int h ) override;
    void paintGL() override;
    void updateInputState();

private:
    Project *mProjectHandle;
    Grid *mGridHandle;
    SelectionHighlighter *mSelectionHighlighterHandle;
    RelationshipTree *mRelationshipTreeHandle;
    bool mGridEnabled;
    bool mRelationshipTreeEnabled;
    WindowInputState mInputState;
};

#endif // GLWIDGET_H

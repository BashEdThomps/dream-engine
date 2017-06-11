#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <DreamCore.h>
#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <QOpenGLContext>

#include "../View/GLView/Grid.h"
#include "../View/GLView/SelectionHighlighter.h"
#include "../View/GLView/RelationshipTree.h"

using namespace Dream;

class QOpenGLWindowComponent : public QOpenGLWidget, public IWindowComponent
{
    Q_OBJECT
public:
    QOpenGLWindowComponent(const QSurfaceFormat& format, QWidget* parent = 0 );
    ~QOpenGLWindowComponent();

    bool init() override;
    void updateComponent(Scene*) override;
    void getCurrentDimensions() override;
    void swapBuffers() override;
    void cleanUp(Scene*) override;
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

private:
    Project              *mProjectHandle;
    Grid                 *mGridHandle;
    SelectionHighlighter *mSelectionHighlighterHandle;
    RelationshipTree     *mRelationshipTreeHandle;
    bool mGridEnabled;
    bool mRelationshipTreeEnabled;

    int mMouseLastX;
    int mMouseLastY;
};

#endif // GLWIDGET_H

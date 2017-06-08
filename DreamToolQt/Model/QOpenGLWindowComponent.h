#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <DreamCore.h>
#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include "../View/GLView/Grid.h"

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

protected:
    void initializeGL() override;
    void resizeGL( int w, int h ) override;
    void paintGL() override;

private:
    Project *mProject;
    Grid* mGrid;
};

#endif // GLWIDGET_H

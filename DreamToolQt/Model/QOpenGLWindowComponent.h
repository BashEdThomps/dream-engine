#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <DreamCore.h>
#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <QOpenGLContext>


class QOpenGLWindowComponent : public QOpenGLWidget, public Dream::IWindowComponent
{
    Q_OBJECT
public:
    QOpenGLWindowComponent(const QSurfaceFormat& format, QWidget* parent = 0 );
    ~QOpenGLWindowComponent();

    bool init() override;
    void updateComponent(Dream::Scene*) override;
    void getCurrentDimensions() override;
    void swapBuffers() override;
    void cleanUp(Dream::Scene*) override;

    void setDreamEngine(Dream::DreamEngine* engine);

protected:
    void initializeGL() override;
    void resizeGL( int w, int h ) override;
    void paintGL() override;
private:
    Dream::DreamEngine *mDreamEngine;
};

#endif // GLWIDGET_H

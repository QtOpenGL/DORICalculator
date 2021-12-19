#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <Core/Controller.h>
#include <OpenGL/Data/RegionData.h>
#include <OpenGL/Object/Object.h>
#include <OpenGL/Renderer/LineRenderer.h>
#include <OpenGL/Renderer/ObjectRenderer.h>
#include <OpenGL/Renderer/RegionRenderer.h>

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QOpenGLWindow>
#include <QTimer>
#include <QWidget>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget *parent = nullptr);
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void refresh();
    void setParameters(Controller::OpenGLWidgetParameters *newParameters);

protected:
    virtual void keyPressEvent(QKeyEvent *) override;
    virtual void keyReleaseEvent(QKeyEvent *) override;
    virtual void mousePressEvent(QMouseEvent *) override;
    virtual void mouseReleaseEvent(QMouseEvent *) override;
    virtual void mouseMoveEvent(QMouseEvent *) override;

private:
    Controller::OpenGLWidgetParameters *mParameters;
    ObjectRenderer *mObjectRenderer;
    RegionRenderer *mRegionRenderer;
    LineRenderer *mLineRenderer;

    QVector<Object *> mObjects;
    RegionData mRegionData[7];

    Object *mCameraObject;

    Camera *mCamera;
    Light *mLight;
    QTimer mTimer;

    QPointF mPreviousMousePosition;
    bool mMousePressed;
    bool mInit;
};

#endif // OPENGLWIDGET_H

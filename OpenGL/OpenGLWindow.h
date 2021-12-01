#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <Core/Controller.h>
#include <OpenGL/Data/RegionData.h>
#include <OpenGL/Object/Object.h>
#include <OpenGL/Renderer/ObjectRenderer.h>

#include <QOpenGLFunctions>
#include <QOpenGLWindow>
#include <QTimer>
#include <QWidget>

class OpenGLWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWindow();
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void refresh();

    Controller::OpenGLWindowParameters *parameters() const;
    void setParameters(Controller::OpenGLWindowParameters *newParameters);

protected:
    virtual void keyPressEvent(QKeyEvent *) override;
    virtual void keyReleaseEvent(QKeyEvent *) override;
    virtual void mousePressEvent(QMouseEvent *) override;
    virtual void mouseReleaseEvent(QMouseEvent *) override;
    virtual void mouseMoveEvent(QMouseEvent *) override;

private:
    Controller::OpenGLWindowParameters *mParameters;
    ObjectRenderer *mObjectRenderer;
    QVector<Object *> mObjects;

    RegionData mRegions[7];

    Camera *mCamera;
    Light *mLight;
    QTimer mTimer;

    QPointF mPreviousMousePosition;
    bool mMousePressed;
    bool mInit;
};

#endif // OPENGLWINDOW_H

#ifndef OPENGLWINDOW3D_H
#define OPENGLWINDOW3D_H

#include "Camera.h"
#include "Renderer/BasicObjectRenderer.h"

#include <QOpenGLFunctions>
#include <QOpenGLWindow>
#include <QTimer>
#include <QWidget>

#include <OpenGL/Renderer/ModelRenderer.h>

class OpenGLWindow3D : public QOpenGLWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWindow3D();
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void createBasicObjects();
    void createModels();

protected:
    virtual void keyPressEvent(QKeyEvent *) override;
    virtual void keyReleaseEvent(QKeyEvent *) override;
    virtual void mousePressEvent(QMouseEvent *) override;
    virtual void mouseReleaseEvent(QMouseEvent *) override;
    virtual void mouseMoveEvent(QMouseEvent *) override;

private:
    BasicObjectRenderer *mBasicObjectRenderer;
    ModelRenderer *mModelRenderer;
    QVector<Node *> mNodes;

    Camera *mCamera;
    Light *mLight;
    QTimer mTimer;

    QPointF mPreviousMousePosition;
    bool mMousePressed;
};

#endif // OPENGLWINDOW3D_H

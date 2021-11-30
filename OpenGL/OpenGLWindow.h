#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include "SceneManager.h"

#include <QOpenGLFunctions>
#include <QOpenGLWindow>
#include <QWidget>

class OpenGLWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWindow();
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

protected:
    virtual void keyPressEvent(QKeyEvent *) override;
    virtual void keyReleaseEvent(QKeyEvent *) override;
    virtual void mousePressEvent(QMouseEvent *) override;
    virtual void mouseReleaseEvent(QMouseEvent *) override;
    virtual void mouseMoveEvent(QMouseEvent *) override;

private:
    SceneManager *mSceneManager;
};

#endif // OPENGLWINDOW_H

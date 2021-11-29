#include "OpenGLWindow3D.h"

#include <QDir>
#include <QKeyEvent>
#include <QMouseEvent>

#include <OpenGL/Node/Model.h>

OpenGLWindow3D::OpenGLWindow3D() {}

void OpenGLWindow3D::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);

    mSceneManager = new SceneManager;
    mSceneManager->init();

    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
}

void OpenGLWindow3D::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mSceneManager->render();
}

void OpenGLWindow3D::resizeGL(int w, int h)
{
    mSceneManager->resize(w, h);
}

void OpenGLWindow3D::keyPressEvent(QKeyEvent *event)
{
    mSceneManager->keyPressEvent(event);
}

void OpenGLWindow3D::keyReleaseEvent(QKeyEvent *event)
{
    mSceneManager->keyReleaseEvent(event);
}

void OpenGLWindow3D::mousePressEvent(QMouseEvent *event)
{
    mSceneManager->mousePressEvent(event);
}

void OpenGLWindow3D::mouseReleaseEvent(QMouseEvent *event)
{
    mSceneManager->mouseReleaseEvent(event);
}

void OpenGLWindow3D::mouseMoveEvent(QMouseEvent *event)
{
    mSceneManager->mouseMoveEvent(event);
}

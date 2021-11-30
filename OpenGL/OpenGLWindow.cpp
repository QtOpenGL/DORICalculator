#include "OpenGLWindow.h"

#include <QDir>
#include <QKeyEvent>
#include <QMouseEvent>

OpenGLWindow::OpenGLWindow() {}

void OpenGLWindow::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);

    mSceneManager = new SceneManager;
    mSceneManager->init();

    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
}

void OpenGLWindow::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mSceneManager->render();
}

void OpenGLWindow::resizeGL(int w, int h) { mSceneManager->resize(w, h); }

void OpenGLWindow::keyPressEvent(QKeyEvent *event) { mSceneManager->keyPressEvent(event); }

void OpenGLWindow::keyReleaseEvent(QKeyEvent *event) { mSceneManager->keyReleaseEvent(event); }

void OpenGLWindow::mousePressEvent(QMouseEvent *event) { mSceneManager->mousePressEvent(event); }

void OpenGLWindow::mouseReleaseEvent(QMouseEvent *event) { mSceneManager->mouseReleaseEvent(event); }

void OpenGLWindow::mouseMoveEvent(QMouseEvent *event) { mSceneManager->mouseMoveEvent(event); }

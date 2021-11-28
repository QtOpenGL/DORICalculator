#include "OpenGLWindow3D.h"

#include <QKeyEvent>
#include <QMouseEvent>

OpenGLWindow3D::OpenGLWindow3D()
    : mMousePressed(false)
{}

void OpenGLWindow3D::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);

    mCuboidRenderer = new CuboidRenderer;

    mCuboidRenderer->initialize();

    mObjects << new Cuboid();
    mObjects << new Cuboid(3, 3, 3);
    mObjects << new Cuboid(-5, -5, -5);
    mObjects << new Cuboid(-2, 2, 2);

    mCamera.setPosition(0, 0, -10);
    mLight.setPosition(2, 3, -10);

    connect(&mTimer, &QTimer::timeout, this, [=]() {
        update();
        mCamera.update();
    });
    mTimer.start(10);
}

void OpenGLWindow3D::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mCuboidRenderer->render(mObjects, mCamera, mLight);
}

void OpenGLWindow3D::resizeGL(int w, int h)
{
    QMatrix4x4 projection;
    projection.setToIdentity();
    projection.perspective(45.0f, float(w) / float(h), 0.1f, 10000.0f);
    mCamera.setProjectionMatrix(projection);
    update();
}

void OpenGLWindow3D::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W)
        mCamera.move(Camera::KeyboardControl::FORWARD, 0.025);
    if (event->key() == Qt::Key_S)
        mCamera.move(Camera::KeyboardControl::BACKWARD, 0.025);
    if (event->key() == Qt::Key_A)
        mCamera.move(Camera::KeyboardControl::LEFT, 0.025);
    if (event->key() == Qt::Key_D)
        mCamera.move(Camera::KeyboardControl::RIGHT, 0.025);
    if (event->key() == Qt::Key_Q)
        mCamera.move(Camera::KeyboardControl::DOWN, 0.025);
    if (event->key() == Qt::Key_E)
        mCamera.move(Camera::KeyboardControl::UP, 0.025);
    if (event->key() == Qt::Key_Shift)
        mCamera.setMovementSpeed(5.0f);
}

void OpenGLWindow3D::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W)
        mCamera.move(Camera::KeyboardControl::FORWARD, 0);
    if (event->key() == Qt::Key_S)
        mCamera.move(Camera::KeyboardControl::BACKWARD, 0);
    if (event->key() == Qt::Key_A)
        mCamera.move(Camera::KeyboardControl::LEFT, 0);
    if (event->key() == Qt::Key_D)
        mCamera.move(Camera::KeyboardControl::RIGHT, 0);
    if (event->key() == Qt::Key_Q)
        mCamera.move(Camera::KeyboardControl::DOWN, 0);
    if (event->key() == Qt::Key_E)
        mCamera.move(Camera::KeyboardControl::UP, 0);
    if (event->key() == Qt::Key_Shift)
        mCamera.setMovementSpeed(1.0f);
}

void OpenGLWindow3D::mousePressEvent(QMouseEvent *)
{
    mMousePressed = true;
}

void OpenGLWindow3D::mouseReleaseEvent(QMouseEvent *)
{
    mMousePressed = false;
    mCamera.rotate(Camera::MouseControl::PAN, 0);
    mCamera.rotate(Camera::MouseControl::TILT, 0);
}

void OpenGLWindow3D::mouseMoveEvent(QMouseEvent *event)
{
    if (mMousePressed) {
        float panAmount = event->pos().x() - mPreviousMousePosition.x();
        float tiltAmount = mPreviousMousePosition.y() - event->pos().y();
        mCamera.rotate(Camera::MouseControl::PAN, panAmount / 4.0f);
        mCamera.rotate(Camera::MouseControl::TILT, tiltAmount / 4.0f);
    }

    mPreviousMousePosition = event->pos();
}

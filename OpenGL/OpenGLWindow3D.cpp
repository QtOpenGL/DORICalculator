#include "OpenGLWindow3D.h"

#include <QKeyEvent>
#include <QMouseEvent>

#include <OpenGL/Object/Plane.h>

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
    mPlaneRenderer = new PlaneRenderer;

    mCuboidRenderer->initialize();
    mPlaneRenderer->initialize();

    mObjects << new Cuboid(0, 0, 0);
    mObjects[0]->setColor(1, 0, 0);

    mObjects << new Cuboid(0, 0, 3);
    mObjects[1]->setColor(1, 1, 0);

    mObjects << new Cuboid(2, 0, 0);
    mObjects[2]->setColor(1, 0, 1);

    mObjects << new Cuboid(0, 2, 0);
    mObjects[3]->setColor(0, 1, 0);

    mObjects << new Plane(2, -2, 0);
    mObjects[4]->setColor(1, 1, 1);
    mObjects[4]->scale(20);

    mCamera = new Camera;
    mLight = new Light;
    mCamera->setPosition(0, 0, 5);
    mLight->setPosition(5, 15, 0);

    connect(&mTimer, &QTimer::timeout, this, [=]() {
        update();
        mCamera->update();

        if (mObjects[0]) {
            QQuaternion dr = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), 0.5);
            dr = dr * QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), 0.25);
            mObjects[0]->rotate(dr);
        }
    });
    mTimer.start(10);
}

void OpenGLWindow3D::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mCuboidRenderer->render(mObjects, mCamera, mLight);
    mPlaneRenderer->render(mObjects, mCamera, mLight);
}

void OpenGLWindow3D::resizeGL(int w, int h)
{
    QMatrix4x4 projection;
    projection.setToIdentity();
    projection.perspective(45.0f, float(w) / float(h), 0.1f, 10000.0f);
    mCamera->setProjectionMatrix(projection);
}

void OpenGLWindow3D::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W)
        mCamera->move(Camera::KeyboardControl::FORWARD, 0.025f);
    if (event->key() == Qt::Key_S)
        mCamera->move(Camera::KeyboardControl::BACKWARD, 0.025f);
    if (event->key() == Qt::Key_A)
        mCamera->move(Camera::KeyboardControl::LEFT, 0.025f);
    if (event->key() == Qt::Key_D)
        mCamera->move(Camera::KeyboardControl::RIGHT, 0.025f);
    if (event->key() == Qt::Key_Q)
        mCamera->move(Camera::KeyboardControl::DOWN, 0.025f);
    if (event->key() == Qt::Key_E)
        mCamera->move(Camera::KeyboardControl::UP, 0.025f);
    if (event->key() == Qt::Key_Shift)
        mCamera->setMovementSpeed(5.0f);
}

void OpenGLWindow3D::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W)
        mCamera->move(Camera::KeyboardControl::FORWARD, 0);
    if (event->key() == Qt::Key_S)
        mCamera->move(Camera::KeyboardControl::BACKWARD, 0);
    if (event->key() == Qt::Key_A)
        mCamera->move(Camera::KeyboardControl::LEFT, 0);
    if (event->key() == Qt::Key_D)
        mCamera->move(Camera::KeyboardControl::RIGHT, 0);
    if (event->key() == Qt::Key_Q)
        mCamera->move(Camera::KeyboardControl::DOWN, 0);
    if (event->key() == Qt::Key_E)
        mCamera->move(Camera::KeyboardControl::UP, 0);
    if (event->key() == Qt::Key_Shift)
        mCamera->setMovementSpeed(1.0f);
}

void OpenGLWindow3D::mousePressEvent(QMouseEvent *)
{
    mMousePressed = true;
}

void OpenGLWindow3D::mouseReleaseEvent(QMouseEvent *)
{
    mMousePressed = false;
    mCamera->rotate(Camera::MouseControl::PAN, 0);
    mCamera->rotate(Camera::MouseControl::TILT, 0);
}

void OpenGLWindow3D::mouseMoveEvent(QMouseEvent *event)
{
    if (mMousePressed) {
        float panAmount = event->pos().x() - mPreviousMousePosition.x();
        float tiltAmount = mPreviousMousePosition.y() - event->pos().y();
        mCamera->rotate(Camera::MouseControl::PAN, panAmount / 4.0f);
        mCamera->rotate(Camera::MouseControl::TILT, tiltAmount / 4.0f);
    }

    mPreviousMousePosition = event->pos();
}

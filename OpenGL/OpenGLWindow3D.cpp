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

    mBasicObjects << new BasicObject(BasicObject::Plane);
    mBasicObjects[0]->setPosition(0, 0, 0);
    mBasicObjects[0]->setColor(1, 1, 1);
    mBasicObjects[0]->scale(100);

    mBasicObjects << new BasicObject(BasicObject::Cuboid);
    mBasicObjects[1]->setPosition(0, 5, -1);
    mBasicObjects[1]->setColor(1, 0, 0);

    mBasicObjects << new BasicObject(BasicObject::Cuboid);
    mBasicObjects[2]->setPosition(5, 5, -3);
    mBasicObjects[2]->setColor(1, 0, 1);

    mBasicObjects << new BasicObject(BasicObject::Cuboid);
    mBasicObjects[3]->setPosition(-2, 2, -3);
    mBasicObjects[3]->setColor(1, 1, 0);

    mBasicObjects << new BasicObject(BasicObject::Cuboid);
    mBasicObjects[4]->setPosition(2, 2, -7);
    mBasicObjects[4]->setColor(1, 1, 1);

    mBasicObjects << new BasicObject(BasicObject::Cuboid);
    mBasicObjects[5]->setPosition(2, 4, -5);
    mBasicObjects[5]->setColor(0, 1, 1);

    mBasicObjects << new BasicObject(BasicObject::Cuboid);
    mBasicObjects[6]->setPosition(3, 7, -5);
    mBasicObjects[6]->setColor(0, 1, 0);

    mBasicObjectRenderer = new BasicObjectRenderer;
    mBasicObjectRenderer->init();

    mCamera = new Camera;
    mLight = new Light;
    mCamera->setPosition(0, 5, 5);
    mLight->setPosition(5, 15, 0);

    connect(&mTimer, &QTimer::timeout, this, [=]() {
        mCamera->update();
        QQuaternion dr = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), 0.5);
        dr = dr * QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), 0.25);
        mBasicObjects[1]->rotate(dr);
    });
    mTimer.start(10);

    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
}

void OpenGLWindow3D::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mBasicObjectRenderer->render(mBasicObjects, mCamera, mLight);
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
    if (event->key() == Qt::Key_Control)
        mCamera->setMovementSpeed(0.25f);
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
    if (event->key() == Qt::Key_Control)
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

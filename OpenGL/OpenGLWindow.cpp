#include "OpenGLWindow.h"

#include <QDir>
#include <QKeyEvent>
#include <QMouseEvent>

OpenGLWindow::OpenGLWindow()
    : mInit(false)
{
    Object *plane = new Object(Object::Type::Plane);
    plane->setPosition(0, -0.1, 0);
    plane->setColor(1, 1, 1);
    plane->scale(10);
    mObjects << plane;

    //    for (int i = 0; i < 10; i++) {
    //        Object *cube = new Object(Object::Type::Cube);
    //        cube->setPosition(10 * i, 1, -5);
    //        cube->setColor(1, 1, 1);
    //        cube->scale(0.01);
    //        mObjects << cube;
    //    }

    //    Object *suzanne = new Object(Object::Type::Suzanne);
    //    suzanne->setPosition(10, 10, 10);
    //    suzanne->setColor(1, 0, 1);
    //    mObjects << suzanne;

    mCameraObject = new Object(Object::Type::Camera);
    mCameraObject->setPosition(-10, 0, 0);
    mCameraObject->setColor(1, 1, 1);
    mObjects << mCameraObject;

    mCamera = new Camera;
    mLight = new Light;
    mCamera->setPosition(0, 20, 20);
    mLight->setPosition(2, 50, 2);
    mLight->setColor(1.0f, 1.0f, 1.0f);
}

void OpenGLWindow::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    //    glDisable(GL_CULL_FACE);
    //    glEnable(GL_BLEND);
    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);

    mObjectRenderer = new ObjectRenderer;
    mObjectRenderer->init();

    mRegionRenderer = new RegionRenderer;
    mRegionRenderer->init();

    mLineRenderer = new LineRenderer;
    mLineRenderer->init();

    for (int i = 0; i < 7; i++) {
        mRegionData[i].create();
    }

    connect(&mTimer, &QTimer::timeout, this, [=]() { mCamera->update(); });
    mTimer.start(10);

    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));

    mInit = true;
}

void OpenGLWindow::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mObjectRenderer->render(mObjects, mCamera, mLight);
    mRegionRenderer->render(mRegionData, mCamera, mLight);
    mLineRenderer->render(mParameters->frustumEdgeVertices, mCamera);
}

void OpenGLWindow::resizeGL(int w, int h)
{
    QMatrix4x4 projection;
    projection.setToIdentity();
    projection.perspective(60.0f, float(w) / float(h), 0.1f, 10000.0f);
    mCamera->setProjectionMatrix(projection);
}

void OpenGLWindow::refresh()
{
    for (int i = 0; i < 7; i++) {
        mRegionData[i].setVertices(mParameters->regions[i].vertices);
        mRegionData[i].setIntersectsGround(mParameters->regions[i].intersectsGround);
    }

    QQuaternion quat = QQuaternion::fromAxisAndAngle(QVector3D(0, 0, -1), mParameters->tiltAngle);
    mCameraObject->setRotation(quat);

    QVector3D cameraForward = quat * QVector3D(1, 0, 0);
    QVector3D position = QVector3D(0, mParameters->cameraHeight, 0) - 0.5 * cameraForward;
    mCameraObject->setPosition(position);
}

void OpenGLWindow::keyPressEvent(QKeyEvent *event)
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

void OpenGLWindow::keyReleaseEvent(QKeyEvent *event)
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

void OpenGLWindow::mousePressEvent(QMouseEvent *) { mMousePressed = true; }

void OpenGLWindow::mouseReleaseEvent(QMouseEvent *)
{
    mMousePressed = false;
    mCamera->rotate(Camera::MouseControl::PAN, 0);
    mCamera->rotate(Camera::MouseControl::TILT, 0);
}

void OpenGLWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (mMousePressed) {
        float panAmount = event->pos().x() - mPreviousMousePosition.x();
        float tiltAmount = mPreviousMousePosition.y() - event->pos().y();
        mCamera->rotate(Camera::MouseControl::PAN, panAmount / 4.0f);
        mCamera->rotate(Camera::MouseControl::TILT, tiltAmount / 4.0f);
    }

    mPreviousMousePosition = event->pos();
}

Controller::OpenGLWindowParameters *OpenGLWindow::parameters() const { return mParameters; }

void OpenGLWindow::setParameters(Controller::OpenGLWindowParameters *newParameters) { mParameters = newParameters; }

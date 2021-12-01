#include "OpenGLWindow.h"

#include <QDir>
#include <QKeyEvent>
#include <QMouseEvent>

OpenGLWindow::OpenGLWindow()
    : mInit(false)
{}

void OpenGLWindow::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);

    mRenderer = new Renderer;
    mRenderer->init();

    //    for (int i = 0; i < 7; i++) {
    //        QVector<QVector3D> fakeVertices;
    //        QVector<QVector3D> fakeNormals = QVector<QVector3D>(36, QVector3D(0, 1, 0));

    //        for (int j = 0; j < 12; ++j) {
    //            fakeVertices << i * QVector3D(1, 1, 1);
    //            fakeVertices << i * QVector3D(-1, 1, 1);
    //            fakeVertices << i * QVector3D(1, 1, -1);
    //        }

    //        Region *region = new Region;
    //        RegionData *data = new RegionData;

    //        data->setVertices(fakeVertices);
    //        data->setNormals(fakeNormals);

    //        if (data->create()) {
    //            region->setData(data);

    //            mRegionData << data;
    //            mObjects << region;

    //            region->setColor(i * 0.1, 1 - i * 0.1, i * i / 100.0f);

    //        } else {
    //            delete data;
    //            delete region;
    //        }
    //    }

    for (int i = 0; i < 7; i++) {
        mRegions[i].data()->create();
    }

    Model *plane = new Model(Model::Type::Plane);
    plane->setPosition(0, -0.01, 0);
    plane->setColor(1, 1, 1);
    plane->scale(10);
    mObjects << plane;

    for (int i = 0; i < 10; i++) {
        Model *cube = new Model(Model::Type::Cube);
        cube->setPosition(10 * i, 1, -5);
        cube->setColor(1, 1, 1);
        cube->scale(0.01);
        mObjects << cube;
    }

    //    Model *suzanne = new Model(Model::Type::Suzanne);
    //    suzanne->setPosition(-10, 10, 10);
    //    suzanne->setColor(1, 0, 1);
    //    mObjects << suzanne;

    mCamera = new Camera;
    mLight = new Light;
    mCamera->setPosition(0, 50, 0);
    mLight->setPosition(2, 50, 2);
    mLight->setColor(1.0f, 1.0f, 1.0f);

    connect(&mTimer, &QTimer::timeout, this, [=]() {
        mCamera->update();
        QQuaternion dr = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), 0.5);
        //mObjects.last()->rotate(dr);
    });
    mTimer.start(10);

    connect(&mSlowTimer, &QTimer::timeout, this, [=]() {
        //        for (int i = 0; i < 7; i++) {
        //            QVector<QVector3D> fakeVertices;
        //            QVector3D randomTranslation = 4 * QVector3D((float(rand()) / RAND_MAX), 0, (float(rand()) / RAND_MAX));
        //            for (int j = 0; j < 12; ++j) {
        //                fakeVertices << randomTranslation + i * QVector3D(1, 1, 1);
        //                fakeVertices << randomTranslation + i * QVector3D(-1, 1, 1);
        //                fakeVertices << randomTranslation + i * QVector3D(1, 1, -1);
        //            }

        //            mRegionData[i]->setVertices(fakeVertices);
        //        }
    });
    mSlowTimer.start(2000);

    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));

    mInit = true;
}

void OpenGLWindow::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mRenderer->render(mObjects, mCamera, mLight);
}

void OpenGLWindow::resizeGL(int w, int h)
{
    QMatrix4x4 projection;
    projection.setToIdentity();
    projection.perspective(45.0f, float(w) / float(h), 0.1f, 10000.0f);
    mCamera->setProjectionMatrix(projection);
}

void OpenGLWindow::refresh()
{
    for (int i = 0; i < 7; i++) {
        mRegions[i].data()->setVertices(mParameters->regions[i].vertices);
        mRegions[i].data()->setNormals(mParameters->regions[i].normals);
        mRegions[i].setColor(mParameters->regions[i].color);
        mRegions[i].setVisible(mParameters->regions[i].visible);
    }

    //    for (int i = 0; i < 7; i++) {
    //        mRegions[i].data()->setVertices(mParameters->regions[i].groundIntersections);
    //        mRegions[i].data()->setNormals(mParameters->regions[i].normals);
    //        mRegions[i].setColor(mParameters->regions[i].color);
    //        mRegions[i].setVisible(mParameters->regions[i].visible);
    //    }
}

void OpenGLWindow::init()
{
    for (int i = 0; i < 7; i++) {
        RegionData *data = new RegionData;
        mRegions[i].setData(data);
        mObjects << &mRegions[i];
    }
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

void OpenGLWindow::mousePressEvent(QMouseEvent *event) { mMousePressed = true; }

void OpenGLWindow::mouseReleaseEvent(QMouseEvent *event)
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

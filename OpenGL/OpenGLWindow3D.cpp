#include "OpenGLWindow3D.h"

#include <QDir>
#include <QKeyEvent>
#include <QMouseEvent>

#include <OpenGL/Node/Model.h>

OpenGLWindow3D::OpenGLWindow3D()
    : mMousePressed(false)
{}

void OpenGLWindow3D::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);

    mBasicObjectRenderer = new BasicObjectRenderer;
    mBasicObjectRenderer->init();

    mModelRenderer = new ModelRenderer;
    mModelRenderer->init();

    mCamera = new Camera;
    mLight = new Light;
    mCamera->setPosition(0, 5, 5);
    mLight->setPosition(0, 10, 15);

    createBasicObjects();
    createModels();

    // Load Suzanne
    ModelData *data = mModelRenderer->loadModel("Resources/Models/Suzanne.obj");

    if (data) {
        Model *model = new Model(data->name());
        model->setPosition(0, 10, -4);
        model->setColored(true);
        model->setColor(1, 1, 1);
        mNodes << model;
    }

    connect(&mTimer, &QTimer::timeout, this, [=]() {
        mCamera->update();
        QQuaternion dr = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), 0.5);
        mNodes.last()->rotate(dr);
    });
    mTimer.start(10);

    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
}

void OpenGLWindow3D::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mBasicObjectRenderer->render(mNodes, mCamera, mLight);
    mModelRenderer->render(mNodes, mCamera, mLight);
}

void OpenGLWindow3D::resizeGL(int w, int h)
{
    QMatrix4x4 projection;
    projection.setToIdentity();
    projection.perspective(45.0f, float(w) / float(h), 0.1f, 10000.0f);
    mCamera->setProjectionMatrix(projection);
}

void OpenGLWindow3D::createBasicObjects()
{
    {
        BasicObject *object = new BasicObject(BasicObject::Plane);
        object->setPosition(0, 0, 0);
        object->setColor(1, 1, 1);
        object->scale(100);
        mNodes << object;
    }

    {
        BasicObject *object = new BasicObject(BasicObject::Cuboid);
        object->setPosition(0, 5, -1);
        object->setColor(1, 0, 0);
        mNodes << object;
    }

    {
        BasicObject *object = new BasicObject(BasicObject::Cuboid);
        object->setPosition(5, 5, -3);
        object->setColor(1, 0, 1);
        mNodes << object;
    }

    {
        BasicObject *object = new BasicObject(BasicObject::Cuboid);
        object->setPosition(-2, 2, -3);
        object->setColor(1, 1, 0);
        mNodes << object;
    }

    {
        BasicObject *object = new BasicObject(BasicObject::Cuboid);
        object->setPosition(2, 2, -7);
        object->setColor(1, 1, 1);
        mNodes << object;
    }

    {
        BasicObject *object = new BasicObject(BasicObject::Cuboid);
        object->setPosition(2, 4, -5);
        object->setColor(0, 1, 1);
        mNodes << object;
    }

    {
        BasicObject *object = new BasicObject(BasicObject::Cuboid);
        object->setPosition(3, 7, -5);
        object->setColor(0, 1, 0);
        mNodes << object;
    }
}

void OpenGLWindow3D::createModels()
{
    QDir dir("Resources/Models/Unscaled");
    QStringList files = dir.entryList(QStringList("*.obj"));
    qInfo() << "Loading models...";
    qInfo() << QString("Total number of .obj files is %1 under the folder '%2'").arg(files.size()).arg(dir.absolutePath());
    qInfo() << "Models are as follows:";
    qInfo() << files;

    for (const auto &file : files) {
        mModelRenderer->loadModel(dir.absoluteFilePath(file));
    }

    QStringList modelNames = mModelRenderer->getModelNames();

    for (int i = 0; i < modelNames.size(); i++) {
        Model *model = new Model(modelNames[i]);
        model->setPosition(3 * modelNames.size() / 2 - 3 * i, 5, 4);
        model->scale(0.01);
        model->setColored(true);
        model->setColor(1, 0, 1);
        mNodes << model;
    }
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

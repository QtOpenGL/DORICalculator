#include "SceneManager.h"

#include <OpenGL/Node/Model.h>
#include <OpenGL/Renderer/BasicObjectRenderer.h>
#include <OpenGL/Renderer/ModelRenderer.h>
#include <OpenGL/Renderer/RegionRenderer.h>

#include <QDir>

SceneManager::SceneManager(QObject *parent)
    : QObject(parent)
{}

bool SceneManager::init()
{
    mBasicObjectRenderer = new BasicObjectRenderer;

    if (!mBasicObjectRenderer->init())
        return false;

    mModelRenderer = new ModelRenderer;

    if (!mModelRenderer->init())
        return false;

    mRegionRenderer = new RegionRenderer;

    if (!mRegionRenderer->init())
        return false;

    for (int i = 0; i < 7; i++) {
        QVector<QVector3D> fakeVertices;
        QVector<QVector3D> fakeNormals = QVector<QVector3D>(36, QVector3D(0, 1, 0));

        for (int j = 0; j < 12; ++j) {
            fakeVertices << i * QVector3D(1, 1, 1);
            fakeVertices << i * QVector3D(-1, 1, 1);
            fakeVertices << i * QVector3D(1, 1, -1);
        }

        mRegions << new RegionData(fakeVertices, fakeNormals);
        mRegions[i]->create();
        mRegions[i]->setColor(QVector3D(i * 0.1, 1 - i * 0.1, i * i / 100.0f));
    }

    mCamera = new Camera;
    mLight = new Light;
    mCamera->setPosition(0, 5, 5);
    mLight->setPosition(0, 10, 15);

    //createBasicObjects();
    //createModels();

    // Create a plane
    {
        BasicObject *object = new BasicObject(BasicObject::Plane);
        object->setPosition(0, 0, 0);
        object->setColor(1, 1, 1);
        object->scale(100);
        mNodes << object;
    }

    // Load Suzanne
    ModelData *data = mModelRenderer->loadModel("Resources/Models/Suzanne.obj");

    if (data) {
        Model *model = new Model(data->name());
        model->setPosition(0, 10, -4);
        model->setColor(1, 1, 1);
        mNodes << model;
    }

    connect(&mTimer, &QTimer::timeout, this, [=]() {
        mCamera->update();
        QQuaternion dr = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), 0.5);
        mNodes.last()->rotate(dr);
    });
    mTimer.start(10);

    connect(&mSlowTimer, &QTimer::timeout, this, [=]() {
        for (int i = 0; i < 7; i++) {
            QVector<QVector3D> fakeVertices;
            QVector3D randomTranslation = 4 * QVector3D((float(rand()) / RAND_MAX), 0, (float(rand()) / RAND_MAX));
            for (int j = 0; j < 12; ++j) {
                fakeVertices << randomTranslation + i * QVector3D(1, 1, 1);
                fakeVertices << randomTranslation + i * QVector3D(-1, 1, 1);
                fakeVertices << randomTranslation + i * QVector3D(1, 1, -1);
            }

            mRegions[i]->setVertices(fakeVertices);
        }
    });
    mSlowTimer.start(2000);

    return true;
}

void SceneManager::render()
{
    mBasicObjectRenderer->render(mNodes, mCamera, mLight);
    mModelRenderer->render(mNodes, mCamera, mLight);
    mRegionRenderer->render(mRegions, mCamera, mLight);
}

void SceneManager::resize(int w, int h)
{
    QMatrix4x4 projection;
    projection.setToIdentity();
    projection.perspective(45.0f, float(w) / float(h), 0.1f, 10000.0f);
    mCamera->setProjectionMatrix(projection);
}

void SceneManager::keyPressEvent(QKeyEvent *event)
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

void SceneManager::keyReleaseEvent(QKeyEvent *event)
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

void SceneManager::mousePressEvent(QMouseEvent *)
{
    mMousePressed = true;
}

void SceneManager::mouseReleaseEvent(QMouseEvent *)
{
    mMousePressed = false;
    mCamera->rotate(Camera::MouseControl::PAN, 0);
    mCamera->rotate(Camera::MouseControl::TILT, 0);
}

void SceneManager::mouseMoveEvent(QMouseEvent *event)
{
    if (mMousePressed) {
        float panAmount = event->pos().x() - mPreviousMousePosition.x();
        float tiltAmount = mPreviousMousePosition.y() - event->pos().y();
        mCamera->rotate(Camera::MouseControl::PAN, panAmount / 4.0f);
        mCamera->rotate(Camera::MouseControl::TILT, tiltAmount / 4.0f);
    }

    mPreviousMousePosition = event->pos();
}

void SceneManager::createBasicObjects()
{
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

void SceneManager::createModels()
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
        model->setColor(1, 0, 1);
        mNodes << model;
    }
}

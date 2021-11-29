#include "BasicObjectRenderer.h"

BasicObjectRenderer::BasicObjectRenderer()
    : mVertexShaderPath("Shaders/BasicObject/VertexShader.vert")
    , mFragmentShaderPath("Shaders/BasicObject/FragmentShader.frag")
    , mInit(false)
{}

bool BasicObjectRenderer::init()
{
    initializeOpenGLFunctions();
    mShader = new QOpenGLShaderProgram;

    if (!mShader->addShaderFromSourceFile(QOpenGLShader::Vertex, mVertexShaderPath)) {
        qCritical() << QString("Could not load '%1'. Error message is %2.").arg(mVertexShaderPath, mShader->log());
        return false;
    } else {
        qInfo() << QString("'%1' is loaded.").arg(mVertexShaderPath);
    }

    if (!mShader->addShaderFromSourceFile(QOpenGLShader::Fragment, mFragmentShaderPath)) {
        qCritical() << QString("Could not load '%1'. Error message is %2.").arg(mFragmentShaderPath, mShader->log());
        return false;
    } else {
        qInfo() << QString("'%1' is loaded.").arg(mFragmentShaderPath);
    }

    mShader->bindAttributeLocation("vertex", 0);
    mShader->bindAttributeLocation("normal", 1);

    if (!mShader->link()) {
        qCritical() << QString("Could not link shader. Error message : %1").arg(mShader->log());
        return false;
    }

    if (!mShader->bind()) {
        qCritical() << QString("Could not bind shader. Error message : %1").arg(mShader->log());
        return false;
    }

    mModelMatrixLocation = mShader->uniformLocation("modelMatrix");
    mViewMatrixLocation = mShader->uniformLocation("viewMatrix");
    mProjectionMatrixLocation = mShader->uniformLocation("projectionMatrix");
    mObjectColorLocation = mShader->uniformLocation("objectColor");
    mLightColorLocation = mShader->uniformLocation("lightColor");
    mLightPositionLocation = mShader->uniformLocation("lightPosition");
    mAmbientStrengthLocation = mShader->uniformLocation("ambientStrength");
    mCameraPositionLocation = mShader->uniformLocation("cameraPosition");
    mLightPowerLocation = mShader->uniformLocation("lightPower");

    for (BasicObject::Type type : {BasicObject::Cuboid, BasicObject::Plane}) {
        BasicObjectData *data = new BasicObjectData(type);
        data->create();
        mObjectData.insert(type, data);
    }

    mShader->release();

    mInit = true;
    return true;
}

void BasicObjectRenderer::render(QVector<BasicObject *> objects, const Camera *camera, const Light *light)
{
    if (!mInit)
        return;

    mShader->bind();
    mShader->setUniformValue(mProjectionMatrixLocation, camera->projectionMatrix());
    mShader->setUniformValue(mViewMatrixLocation, camera->viewMatrix());
    mShader->setUniformValue(mLightColorLocation, light->color());
    mShader->setUniformValue(mLightPositionLocation, light->position());
    mShader->setUniformValue(mAmbientStrengthLocation, light->ambientStrength());
    mShader->setUniformValue(mCameraPositionLocation, camera->position());
    mShader->setUniformValue(mLightPowerLocation, light->lightPower());

    for (const auto &object : objects) {
        mShader->setUniformValue(mObjectColorLocation, object->color());
        mShader->setUniformValue(mModelMatrixLocation, object->transformationMatrix());

        BasicObjectData *data = mObjectData.value(object->type());
        data->bind();
        glDrawArrays(GL_TRIANGLES, 0, data->getVertexCount());
        data->release();
    }

    mShader->release();
}

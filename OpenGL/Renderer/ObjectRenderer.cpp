#include "ObjectRenderer.h"

#include <Helper.h>

ObjectRenderer::ObjectRenderer()
    : mVertexShaderPath(":/Resources/Shaders/Object/VertexShader.vert")
    , mFragmentShaderPath(":/Resources/Shaders/Object/FragmentShader.frag")
{}

bool ObjectRenderer::init()
{
    initializeOpenGLFunctions();
    mShader = new QOpenGLShaderProgram;

    if (!mShader->addShaderFromSourceCode(QOpenGLShader::Vertex, Helper::getBytes(mVertexShaderPath))) {
        qCritical() << QString("Could not load '%1'. Error message is %2.").arg(mVertexShaderPath, mShader->log());
        return false;
    } else {
        qInfo() << QString("'%1' is loaded.").arg(mVertexShaderPath);
    }

    if (!mShader->addShaderFromSourceCode(QOpenGLShader::Fragment, Helper::getBytes(mFragmentShaderPath))) {
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

    mModelMatrix = mShader->uniformLocation("modelMatrix");
    mViewMatrix = mShader->uniformLocation("viewMatrix");
    mProjectionMatrix = mShader->uniformLocation("projectionMatrix");
    mLightColor = mShader->uniformLocation("lightColor");
    mLightPosition = mShader->uniformLocation("lightPosition");
    mAmbientStrength = mShader->uniformLocation("ambientStrength");
    mCameraPosition = mShader->uniformLocation("cameraPosition");
    mLightPower = mShader->uniformLocation("lightPower");
    mObjectColor = mShader->uniformLocation("objectColor");

    // Load models
    for (int i = 0; i != Object::TorusKnot; i++) {
        Object::Type type = static_cast<Object::Type>(i);
        ObjectData *data = new ObjectData(type);
        data->create();
        mObjectData.insert(type, data);
    }

    mShader->release();

    return mInit = true;
}

void ObjectRenderer::render(QVector<Object *> objects, const Camera *camera, const Light *light)
{
    if (!mInit)
        return;

    mShader->bind();

    mShader->setUniformValue(mProjectionMatrix, camera->projectionMatrix());
    mShader->setUniformValue(mViewMatrix, camera->viewMatrix());
    mShader->setUniformValue(mLightColor, light->color());
    mShader->setUniformValue(mLightPosition, light->position());
    mShader->setUniformValue(mAmbientStrength, light->ambientStrength());
    mShader->setUniformValue(mCameraPosition, camera->position());
    mShader->setUniformValue(mLightPower, light->lightPower());

    for (const auto &object : objects) {
        if (object) {
            ObjectData *data = mObjectData.value(object->type(), nullptr);
            if (data) {
                mShader->setUniformValue(mModelMatrix, object->transformationMatrix());
                mShader->setUniformValue(mObjectColor, object->color());
                data->bind();
                glDrawArrays(GL_TRIANGLES, 0, data->getVertexCount());
                data->release();
            }
        }
    }

    mShader->release();
}

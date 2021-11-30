#include "Renderer.h"

#include <OpenGL/Object/Region.h>

Renderer::Renderer()
    : mVertexShaderPath("Shaders/VertexShader.vert")
    , mFragmentShaderPath("Shaders/FragmentShader.frag")
{}

bool Renderer::init()
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

    // Load models
    for (int i = Model::Capsule; i != Model::TorusKnot; i++) {
        Model::Type type = static_cast<Model::Type>(i);
        ModelData *data = new ModelData(type);
        data->create();
        mModelData.insert(type, data);
    }

    mShader->release();

    return mInit = true;
}

void Renderer::render(QVector<Object *> objects, const Camera *camera, const Light *light)
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
        Model *model = dynamic_cast<Model *>(object);
        Region *region = dynamic_cast<Region *>(object);

        ObjectData *data = nullptr;

        if (model) {
            data = mModelData.value(model->type(), nullptr);
        }

        if (region) {
            data = region->data();
        }

        if (data) {
            mShader->setUniformValue(mModelMatrixLocation, object->transformationMatrix());
            mShader->setUniformValue(mObjectColorLocation, object->color());
            data->bind();
            data->update();
            glDrawArrays(GL_TRIANGLES, 0, data->getVertexCount());
            data->release();
        }
    }

    mShader->release();
}

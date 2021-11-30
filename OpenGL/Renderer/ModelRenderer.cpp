#include "ModelRenderer.h"

#include <OpenGL/Node/Model.h>

ModelRenderer::ModelRenderer()
    : Renderer("Shaders/Model/VertexShader.vert", "Shaders/Model/FragmentShader.frag")
    , mInit(false)
{}

bool ModelRenderer::init()
{
    if (!Renderer::init())
        return false;

    mInit = true;

    return true;
}

void ModelRenderer::render(QVector<Node *> nodes, const Camera *camera, const Light *light)
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

    for (const auto &node : nodes) {
        Model *model = dynamic_cast<Model *>(node);
        if (model) {
            mShader->setUniformValue(mColorLocation, model->color());
            mShader->setUniformValue(mModelMatrixLocation, model->transformationMatrix());

            ModelData *data = mModels.value(model->name(), nullptr);
            if (data) {
                data->bind();
                glDrawArrays(GL_TRIANGLES, 0, data->getVertexCount());
                data->release();
            }
        }
    }

    mShader->release();
}

QStringList ModelRenderer::getModelNames()
{
    return mModels.keys();
}

QString ModelRenderer::getModelNameFromFilename(QString filename)
{
    int index = filename.lastIndexOf(".");
    filename.remove(index, filename.length() - index);

    index = filename.lastIndexOf("/");
    filename.remove(0, index + 1);

    return filename;
}

ModelData *ModelRenderer::loadModel(QString path)
{
    QString modelName = getModelNameFromFilename(path);

    // Check if the model is already in the models map
    {
        ModelData *data = mModels.value(modelName, nullptr);
        if (data) {
            qInfo() << modelName << "is already loaded.";
            return data;
        }
    }

    // Create new model data
    ModelData *data = new ModelData(modelName);

    if (!data->loadDataFromFile(path)) {
        qWarning() << QString("Could not load model data for '%1'.").arg(data->name());
        delete data;
        return nullptr;
    }

    mShader->bind();
    data->create();
    mShader->release();

    mModels.insert(data->name(), data);

    return data;
}

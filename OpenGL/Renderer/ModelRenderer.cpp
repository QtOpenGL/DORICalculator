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

    mShader->bindAttributeLocation("vertex", 0);
    mShader->bindAttributeLocation("normal", 1);
    mShader->bind();

    mShader->bindAttributeLocation("vertex", 0);
    mShader->bindAttributeLocation("normal", 1);

    ModelData *data = new ModelData("Suzanne");

    if (!data->loadDataFromFile("Resources/Models/Suzanne.obj")) {
        qWarning() << QString("Could not load model data for '%1'.").arg(data->name());
        return false;
    }

    data->create();

    mModels.insert(data->name(), data);
    mShader->release();

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

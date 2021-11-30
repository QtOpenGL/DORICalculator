#include "BasicObjectRenderer.h"

BasicObjectRenderer::BasicObjectRenderer()
    : Renderer("Shaders/BasicObject/VertexShader.vert", "Shaders/BasicObject/FragmentShader.frag")
    , mInit(false)
{}

bool BasicObjectRenderer::init()
{
    if (!Renderer::init())
        return false;

    mShader->bindAttributeLocation("vertex", 0);
    mShader->bindAttributeLocation("normal", 1);
    mShader->bind();

    for (BasicObject::Type type : {BasicObject::Cuboid, BasicObject::Plane}) {
        BasicObjectData *data = new BasicObjectData(type);
        data->create();
        mObjectData.insert(type, data);
    }

    mShader->release();

    mInit = true;
    return true;
}

void BasicObjectRenderer::render(QVector<Node *> nodes, const Camera *camera, const Light *light)
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
        BasicObject *object = dynamic_cast<BasicObject *>(node);
        if (object) {
            mShader->setUniformValue(mColorLocation, object->color());
            mShader->setUniformValue(mModelMatrixLocation, object->transformationMatrix());

            BasicObjectData *data = mObjectData.value(object->type(), nullptr);
            if (data) {
                data->bind();
                glDrawArrays(GL_TRIANGLES, 0, data->getVertexCount());
                data->release();
            }
        }
    }

    mShader->release();
}

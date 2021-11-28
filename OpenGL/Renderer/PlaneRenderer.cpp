#include "PlaneRenderer.h"

#include <OpenGL/Object/Plane.h>

PlaneRenderer::PlaneRenderer()
    : Renderer("Shaders/Simple/VertexShader.vert", "Shaders/Simple/FragmentShader.frag")
{}

bool PlaneRenderer::initialize()
{
    if (!Renderer::initialize())
        return false;

    mInitialized = true;
    return true;
}

void PlaneRenderer::render(QVector<Object *> objects, const Camera *camera, const Light *light)
{
    if (!mInitialized)
        return;

    mVertexArray.bind();
    mShader->bind();

    mShader->setUniformValue(mProjectionMatrixLocation, camera->projectionMatrix());
    mShader->setUniformValue(mViewMatrixLocation, camera->viewMatrix());
    mShader->setUniformValue(mLightColorLocation, light->color());
    mShader->setUniformValue(mLightPositionLocation, light->position());
    mShader->setUniformValue(mAmbientStrengthLocation, light->ambientStrength());
    mShader->setUniformValue(mCameraPositionLocation, camera->position());
    mShader->setUniformValue(mLightPowerLocation, light->lightPower());

    for (int i = 0; i < objects.size(); i++) {
        Plane *plane = dynamic_cast<Plane *>(objects[i]);

        if (plane) {
            mShader->setUniformValue(mObjectColorLocation, plane->color());
            mShader->setUniformValue(mModelMatrixLocation, plane->modelMatrix());
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

    mShader->release();
    mVertexArray.release();
}

const float *PlaneRenderer::getVertexData() const
{
    return VERTEX_DATA;
}

const int PlaneRenderer::getVertexDataByteSize() const
{
    return sizeof(VERTEX_DATA);
}

const float PlaneRenderer::VERTEX_DATA[36] = {
    -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, // First triangle vertex(x,y,z), normal(x,y,z)

    -0.5f, 0.0f, 0.5f,  0.0f, 1.0f, 0.0f,

    0.5f,  0.0f, -0.5f, 0.0f, 1.0f, 0.0f,

    0.5f,  0.0f, -0.5f, 0.0f, 1.0f, 0.0f, // Second triangle

    -0.5f, 0.0f, 0.5f,  0.0f, 1.0f, 0.0f,

    0.5f,  0.0f, 0.5f,  0.0f, 1.0f, 0.0f,

};

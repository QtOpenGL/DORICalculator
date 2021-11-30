#include "RegionRenderer.h"

RegionRenderer::RegionRenderer()
    : Renderer("Shaders/Region/VertexShader.vert", "Shaders/Region/FragmentShader.frag")
    , mInit(false)
{}

bool RegionRenderer::init()
{
    if (!Renderer::init())
        return false;

    mShader->bindAttributeLocation("vertex", 0);
    mShader->bindAttributeLocation("normal", 1);

    mInit = true;
    return true;
}

void RegionRenderer::render(QVector<RegionData *> regions, const Camera *camera, const Light *light)
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

    for (const auto &region : regions) {
        mShader->setUniformValue(mColorLocation, region->color());

        if (region) {
            region->bind();
            region->update();
            glDrawArrays(GL_TRIANGLES, 0, region->getVertexCount());
            region->release();
        }
    }

    mShader->release();
}

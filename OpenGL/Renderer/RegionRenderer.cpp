#include "RegionRenderer.h"
#include <Core/Constants.h>
#include <Helper.h>

RegionRenderer::RegionRenderer()
    : mVertexShaderPath(":/Resources/Shaders/Region/VertexShader.vert")
    , mFragmentShaderPath(":/Resources/Shaders/Region/FragmentShader.frag")
{}

bool RegionRenderer::init()
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

    if (!mShader->link()) {
        qCritical() << QString("Could not link shader. Error message : %1").arg(mShader->log());
        return false;
    }

    if (!mShader->bind()) {
        qCritical() << QString("Could not bind shader. Error message : %1").arg(mShader->log());
        return false;
    }

    mViewMatrix = mShader->uniformLocation("viewMatrix");
    mProjectionMatrix = mShader->uniformLocation("projectionMatrix");
    mLightColor = mShader->uniformLocation("lightColor");
    mLightPosition = mShader->uniformLocation("lightPosition");
    mAmbientStrength = mShader->uniformLocation("ambientStrength");
    mCameraPosition = mShader->uniformLocation("cameraPosition");
    mLightPower = mShader->uniformLocation("lightPower");
    mRegionColor = mShader->uniformLocation("regionColor");

    mShader->release();

    return mInit = true;
}

void RegionRenderer::render(RegionData regionData[7], const Camera *camera, const Light *light)
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

    for (int i = 0; i < 7; ++i) {
        if (regionData[i].intersectsGround()) {
            mShader->setUniformValue(mRegionColor,
                                     QVector3D(REGION_COLORS[i].red() / 255.0f, REGION_COLORS[i].green() / 255.0f, REGION_COLORS[i].blue() / 255.0f));
            regionData[i].bind();
            regionData[i].update();
            glDrawArrays(GL_TRIANGLES, 0, regionData[i].getVertexCount());
            regionData[i].release();
        }
    }

    mShader->release();
}

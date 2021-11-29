#include "Renderer.h"

Renderer::Renderer(QString vertexShaderPath, QString fragmentShaderPath)
    : mVertexShaderPath(vertexShaderPath)
    , mFragmentShaderPath(fragmentShaderPath)
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
    mLightColorLocation = mShader->uniformLocation("lightColor");
    mLightPositionLocation = mShader->uniformLocation("lightPosition");
    mAmbientStrengthLocation = mShader->uniformLocation("ambientStrength");
    mCameraPositionLocation = mShader->uniformLocation("cameraPosition");
    mLightPowerLocation = mShader->uniformLocation("lightPower");

    mShader->release();

    return true;
}

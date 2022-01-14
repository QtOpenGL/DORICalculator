#include "LineRenderer.h"

#include <Helper.h>

LineRenderer::LineRenderer()
    : mVertexShaderPath(":/Resources/Shaders/Line/VertexShader.vert")
    , mFragmentShaderPath(":/Resources/Shaders/Line/FragmentShader.frag")
{}

bool LineRenderer::init()
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

    mVertexArray.create();
    mVertexArray.bind();

    mVertexBuffer.create();
    mVertexBuffer.bind();
    mVertexBuffer.setUsagePattern(QOpenGLBuffer::UsagePattern::DynamicDraw);
    mVertexBuffer.allocate(100 * sizeof(QVector3D));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,                 // Size
                          GL_FLOAT,          // Type
                          GL_FALSE,          // Normalized
                          sizeof(QVector3D), // Stride
                          nullptr            // Offset
    );
    mVertexBuffer.release();
    mVertexArray.release();

    mShader->release();

    return mInit = true;
}

void LineRenderer::render(const QVector<QVector3D> &vertices, const Camera *camera)
{
    if (!mInit)
        return;

    mShader->bind();

    mShader->setUniformValue(mProjectionMatrix, camera->projectionMatrix());
    mShader->setUniformValue(mViewMatrix, camera->viewMatrix());

    mVertexArray.bind();
    mVertexBuffer.bind();
    mVertexBuffer.write(0, vertices.constData(), vertices.size() * sizeof(QVector3D));
    mVertexBuffer.release();

    glDrawArrays(GL_LINES, 0, vertices.size());

    mVertexArray.release();
    mShader->release();
}

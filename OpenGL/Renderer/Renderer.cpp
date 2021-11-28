#include "Renderer.h"

Renderer::Renderer(const QString &vertexShaderPath, const QString &fragmentShaderPath)
    : mVertexShaderPath(vertexShaderPath)
    , mFragmentShaderPath(fragmentShaderPath)
    , mShader(nullptr)
    , mModelMatrixLocation(0)
    , mViewMatrixLocation(0)
    , mProjectionMatrixLocation(0)
    , mObjectColorLocation(0)
    , mAmbientStrengthLocation(0)
    , mCameraPositionLocation(0)
    , mLightPositionLocation(0)
    , mLightColorLocation(0)
    , mLightPowerLocation(0)
{}

Renderer::~Renderer()
{
    if (mShader)
        delete mShader;
}

bool Renderer::initialize()
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
    mShader->bindAttributeLocation("normal", 0);

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

    mVertexArray.create();
    mVertexArray.bind();

    mVertexBuffer.create();
    mVertexBuffer.bind();
    mVertexBuffer.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    mVertexBuffer.allocate(getVertexData(), getVertexDataByteSize());

    glEnableVertexAttribArray(0);                      // Vertex atribute location in the shader
    glVertexAttribPointer(0,                           // Vertex atribute location in the shader
                          3,                           // Size
                          GL_FLOAT,                    // Type
                          GL_FALSE,                    // Normalized?
                          6 * sizeof(GLfloat),         // Stride
                          (void *) (0 * sizeof(float)) // Offset
    );

    glEnableVertexAttribArray(1);                      // Vertex atribute location in the shader
    glVertexAttribPointer(1,                           // Vertex atribute location in the shader
                          3,                           // Size
                          GL_FLOAT,                    // Type
                          GL_FALSE,                    // Normalized?
                          6 * sizeof(GLfloat),         // Stride
                          (void *) (3 * sizeof(float)) // Offset
    );

    mVertexBuffer.release();
    mVertexArray.release();
    mShader->release();

    return true;
}

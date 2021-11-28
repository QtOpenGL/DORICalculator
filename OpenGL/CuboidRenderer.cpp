#include "CuboidRenderer.h"
CuboidRenderer::CuboidRenderer()
    : mVertexShaderPath("Shaders/Cuboid/VertexShader.vert")
    , mFragmentShaderPath("Shaders/Cuboid/FragmentShader.frag")
    , mInitialized(false)
{}

bool CuboidRenderer::initialize()
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

    // Always bind attributes before calling link() and bind(). Strange...
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

    mVertexArray.create();
    mVertexArray.bind();

    mVertexBuffer.create();
    mVertexBuffer.bind();
    mVertexBuffer.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    mVertexBuffer.allocate(VERTEX_DATA, sizeof(VERTEX_DATA));

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

    mInitialized = true;
    return true;
}

void CuboidRenderer::render(QVector<Object *> objects, const Camera &camera, const Light &light)
{
    if (!mInitialized)
        return;

    mVertexArray.bind();
    mShader->bind();

    mShader->setUniformValue(mProjectionMatrixLocation, camera.projectionMatrix());
    mShader->setUniformValue(mViewMatrixLocation, camera.viewMatrix());
    mShader->setUniformValue(mLightColorLocation, light.color());
    mShader->setUniformValue(mLightPositionLocation, light.position());
    mShader->setUniformValue(mAmbientStrengthLocation, light.ambientStrength());

    for (int i = 0; i < objects.size(); i++) {
        Cuboid *cuboid = dynamic_cast<Cuboid *>(objects[i]);

        if (cuboid) {
            mShader->setUniformValue(mObjectColorLocation, cuboid->color());
            mShader->setUniformValue(mModelMatrixLocation, cuboid->modelMatrix());
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    mShader->release();
    mVertexArray.release();
}

const float CuboidRenderer::VERTEX_DATA[216] = {-0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
                                                0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,

                                                -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                                                0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,

                                                -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
                                                -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,

                                                0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
                                                0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

                                                -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
                                                0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,

                                                -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                                                0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f};

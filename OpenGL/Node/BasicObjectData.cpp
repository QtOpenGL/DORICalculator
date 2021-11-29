#include "BasicObjectData.h"

BasicObjectData::BasicObjectData(BasicObject::Type type)
    : mType(type)
{}

void BasicObjectData::create()
{
    if (mType == BasicObject::Plane) {
        mVertices = {QVector3D(-0.5f, 0.0f, -0.5f),
                     QVector3D(-0.5f, 0.0f, 0.5f),
                     QVector3D(0.5f, 0.0f, -0.5f),
                     QVector3D(0.5f, 0.0f, -0.5f),
                     QVector3D(-0.5f, 0.0f, 0.5f),
                     QVector3D(0.5f, 0.0f, 0.5f)};

        mNormals = {QVector3D(0.0f, 1.0f, 0.0f),
                    QVector3D(0.0f, 1.0f, 0.0f),
                    QVector3D(0.0f, 1.0f, 0.0f),
                    QVector3D(0.0f, 1.0f, 0.0f),
                    QVector3D(0.0f, 1.0f, 0.0f),
                    QVector3D(0.0f, 1.0f, 0.0f)};

    } else if (mType == BasicObject::Cuboid) {
        mVertices = {QVector3D(-0.5f, -0.5f, -0.5f), QVector3D(0.5f, -0.5f, -0.5f), QVector3D(0.5f, 0.5f, -0.5f), QVector3D(0.5f, 0.5f, -0.5f),  QVector3D(-0.5f, 0.5f, -0.5f),
                     QVector3D(-0.5f, -0.5f, -0.5f), QVector3D(-0.5f, -0.5f, 0.5f), QVector3D(0.5f, -0.5f, 0.5f), QVector3D(0.5f, 0.5f, 0.5f),   QVector3D(0.5f, 0.5f, 0.5f),
                     QVector3D(-0.5f, 0.5f, 0.5f),   QVector3D(-0.5f, -0.5f, 0.5f), QVector3D(-0.5f, 0.5f, 0.5f), QVector3D(-0.5f, 0.5f, -0.5f), QVector3D(-0.5f, -0.5f, -0.5f),
                     QVector3D(-0.5f, -0.5f, -0.5f), QVector3D(-0.5f, -0.5f, 0.5f), QVector3D(-0.5f, 0.5f, 0.5f), QVector3D(0.5f, 0.5f, 0.5f),   QVector3D(0.5f, 0.5f, -0.5f),
                     QVector3D(0.5f, -0.5f, -0.5f),  QVector3D(0.5f, -0.5f, -0.5f), QVector3D(0.5f, -0.5f, 0.5f), QVector3D(0.5f, 0.5f, 0.5f),   QVector3D(-0.5f, -0.5f, -0.5f),
                     QVector3D(0.5f, -0.5f, -0.5f),  QVector3D(0.5f, -0.5f, 0.5f),  QVector3D(0.5f, -0.5f, 0.5f), QVector3D(-0.5f, -0.5f, 0.5f), QVector3D(-0.5f, -0.5f, -0.5f),
                     QVector3D(-0.5f, 0.5f, -0.5f),  QVector3D(0.5f, 0.5f, -0.5f),  QVector3D(0.5f, 0.5f, 0.5f),  QVector3D(0.5f, 0.5f, 0.5f),   QVector3D(-0.5f, 0.5f, 0.5f),
                     QVector3D(-0.5f, 0.5f, -0.5f)};

        mNormals = {QVector3D(0.0f, 0.0f, -1.0f), QVector3D(0.0f, 0.0f, -1.0f), QVector3D(0.0f, 0.0f, -1.0f), QVector3D(0.0f, 0.0f, -1.0f), QVector3D(0.0f, 0.0f, -1.0f),
                    QVector3D(0.0f, 0.0f, -1.0f), QVector3D(0.0f, 0.0f, 1.0f),  QVector3D(0.0f, 0.0f, 1.0f),  QVector3D(0.0f, 0.0f, 1.0f),  QVector3D(0.0f, 0.0f, 1.0f),
                    QVector3D(0.0f, 0.0f, 1.0f),  QVector3D(0.0f, 0.0f, 1.0f),  QVector3D(-1.0f, 0.0f, 0.0f), QVector3D(-1.0f, 0.0f, 0.0f), QVector3D(-1.0f, 0.0f, 0.0f),
                    QVector3D(-1.0f, 0.0f, 0.0f), QVector3D(-1.0f, 0.0f, 0.0f), QVector3D(-1.0f, 0.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f),  QVector3D(1.0f, 0.0f, 0.0f),
                    QVector3D(1.0f, 0.0f, 0.0f),  QVector3D(1.0f, 0.0f, 0.0f),  QVector3D(1.0f, 0.0f, 0.0f),  QVector3D(1.0f, 0.0f, 0.0f),  QVector3D(0.0f, -1.0f, 0.0f),
                    QVector3D(0.0f, -1.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f),
                    QVector3D(0.0f, 1.0f, 0.0f),  QVector3D(0.0f, 1.0f, 0.0f),  QVector3D(0.0f, 1.0f, 0.0f),  QVector3D(0.0f, 1.0f, 0.0f),  QVector3D(0.0f, 1.0f, 0.0f),
                    QVector3D(0.0f, 1.0f, 0.0f)};
    }

    initializeOpenGLFunctions();

    mVertexArray.create();
    mVertexArray.bind();

    mVertexBuffer.create();
    mVertexBuffer.bind();
    mVertexBuffer.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    mVertexBuffer.allocate(mVertices.constData(), sizeof(QVector3D) * mVertices.size());

    glEnableVertexAttribArray(0);              // Vertex atribute location in the shader
    glVertexAttribPointer(0,                   // Vertex atribute location in the shader
                          3,                   // Size
                          GL_FLOAT,            // Type
                          GL_FALSE,            // Normalized?
                          3 * sizeof(GLfloat), // Stride
                          nullptr              // Offset
    );
    mVertexBuffer.release();

    mNormalBuffer.create();
    mNormalBuffer.bind();
    mNormalBuffer.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);
    mNormalBuffer.allocate(mNormals.constData(), sizeof(QVector3D) * mNormals.size());

    glEnableVertexAttribArray(1);              // Vertex atribute location in the shader
    glVertexAttribPointer(1,                   // Vertex atribute location in the shader
                          3,                   // Size
                          GL_FLOAT,            // Type
                          GL_FALSE,            // Normalized?
                          3 * sizeof(GLfloat), // Stride
                          nullptr              // Offset
    );
    mNormalBuffer.release();

    mVertexArray.release();
}

void BasicObjectData::bind()
{
    mVertexArray.bind();
}

void BasicObjectData::release()
{
    mVertexArray.release();
}

int BasicObjectData::getVertexCount()
{
    return mVertices.size();
}

BasicObject::Type BasicObjectData::type()
{
    return mType;
}

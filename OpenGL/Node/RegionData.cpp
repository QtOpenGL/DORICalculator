#include "RegionData.h"

RegionData::RegionData(const QVector<QVector3D> &vertices, const QVector<QVector3D> &normals)
    : mVertices(vertices)
    , mNormals(normals)
{}

void RegionData::create()
{
    initializeOpenGLFunctions();

    mVertexArray.create();
    mVertexArray.bind();

    mVertexBuffer.create();
    mVertexBuffer.bind();
    mVertexBuffer.setUsagePattern(QOpenGLBuffer::UsagePattern::DynamicDraw);
    mVertexBuffer.allocate(getVertexCount() * sizeof(QVector3D));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,                 // Size
                          GL_FLOAT,          // Type
                          GL_FALSE,          // Normalized
                          sizeof(QVector3D), // Stride
                          nullptr            // Offset
    );
    mVertexBuffer.release();

    mNormalBuffer.create();
    mNormalBuffer.bind();
    mNormalBuffer.setUsagePattern(QOpenGLBuffer::UsagePattern::DynamicDraw);
    mNormalBuffer.allocate(getVertexCount() * sizeof(QVector3D));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          3,                 // Size
                          GL_FLOAT,          // Type
                          GL_FALSE,          // Normalized?
                          sizeof(QVector3D), // Stride
                          nullptr            // Offset
    );
    mNormalBuffer.release();

    mVertexArray.release();
}

void RegionData::update()
{
    mVertexBuffer.bind();
    mVertexBuffer.write(0, mVertices.constData(), getVertexCount() * sizeof(QVector3D));
    mVertexBuffer.release();

    mNormalBuffer.bind();
    mNormalBuffer.write(0, mNormals.constData(), getVertexCount() * sizeof(QVector3D));
    mNormalBuffer.release();
}

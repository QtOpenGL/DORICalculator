#include "RegionData.h"

RegionData::RegionData() {}

RegionData::~RegionData() {}

bool RegionData::create()
{
    initializeOpenGLFunctions();

    mVertexArray.create();
    mVertexArray.bind();

    mVertexBuffer.create();
    mVertexBuffer.bind();
    mVertexBuffer.setUsagePattern(QOpenGLBuffer::UsagePattern::DynamicDraw);
    mVertexBuffer.allocate(42 * sizeof(QVector3D)); // Each region has at most 7 faces or 14 triangles or 42 vertices

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
    mNormalBuffer.allocate(42 * sizeof(QVector3D));

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

    return true;
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

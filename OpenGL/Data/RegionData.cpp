#include "RegionData.h"

RegionData::RegionData()
    : mColor(1, 1, 1)
{}

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
    mVertexArray.release();

    return true;
}

void RegionData::update()
{
    mVertexBuffer.bind();
    mVertexBuffer.write(0, mVertices.constData(), getVertexCount() * sizeof(QVector3D));
    mVertexBuffer.release();
}

int RegionData::getVertexCount() { return mVertices.size(); }

const QVector<QVector3D> &RegionData::vertices() const { return mVertices; }

void RegionData::setVertices(const QVector<QVector3D> &newVertices) { mVertices = newVertices; }

const QVector3D &RegionData::color() const { return mColor; }

void RegionData::setColor(const QVector3D &newColor) { mColor = newColor; }

bool RegionData::visible() const { return mVisible; }

void RegionData::setVisible(bool newVisible) { mVisible = newVisible; }

void RegionData::bind() { mVertexArray.bind(); }

void RegionData::release() { mVertexArray.release(); }

#include "RegionData.h"

RegionData::RegionData() {}

bool RegionData::create()
{
    initializeOpenGLFunctions();

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

bool RegionData::intersectsGround() const { return mIntersectsGround; }

void RegionData::setIntersectsGround(bool newIntersectsGround) { mIntersectsGround = newIntersectsGround; }

void RegionData::bind() { mVertexArray.bind(); }

void RegionData::release() { mVertexArray.release(); }

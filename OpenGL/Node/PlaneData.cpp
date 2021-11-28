#include "PlaneData.h"

PlaneData::PlaneData() {}

void PlaneData::createVertexData()
{
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
}

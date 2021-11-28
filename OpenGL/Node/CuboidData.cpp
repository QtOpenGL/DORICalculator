#include "CuboidData.h"

#include <QVector3D>

CuboidData::CuboidData() {}

void CuboidData::createVertexData()
{
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

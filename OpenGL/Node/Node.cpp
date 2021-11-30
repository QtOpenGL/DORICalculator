#include "Node.h"

Node::Node()
    : mPosition(0, 0, 0)
    , mColor(1, 1, 1)
{}

void Node::setTransformationMatrix(const QMatrix4x4 &newTransformationMatrix)
{
    mTransformationMatrix = newTransformationMatrix;
    QVector4D translation = newTransformationMatrix.column(3);
    mPosition = QVector3D(translation.x(), translation.y(), translation.z());
}

void Node::setPosition(float x, float y, float z)
{
    mTransformationMatrix.setColumn(3, QVector4D(x, y, z, 1));
    mPosition = QVector3D(x, y, z);
}

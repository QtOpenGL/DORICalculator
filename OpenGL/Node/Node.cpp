#include "Node.h"

Node::Node() {}

const QMatrix4x4 &Node::transformationMatrix() const
{
    return mTransformationMatrix;
}

void Node::setTransformationMatrix(const QMatrix4x4 &newTransformationMatrix)
{
    mTransformationMatrix = newTransformationMatrix;
    QVector4D translation = newTransformationMatrix.column(3);
    mPosition = QVector3D(translation.x(), translation.y(), translation.z());
}

const QVector3D &Node::position() const
{
    return mPosition;
}

void Node::setPosition(float x, float y, float z)
{
    mTransformationMatrix.setColumn(3, QVector4D(x, y, z, 1));
    mPosition = QVector3D(x, y, z);
}

void Node::rotate(const QQuaternion &deltaRotation)
{
    mTransformationMatrix.rotate(deltaRotation);
}

void Node::scale(float x, float y, float z)
{
    mTransformationMatrix.scale(x, y, z);
}

void Node::scale(float factor)
{
    mTransformationMatrix.scale(factor);
}

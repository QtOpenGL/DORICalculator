#include "Object.h"

Object::Object(float x, float y, float z)
{
    mModelMatrix.setToIdentity();
    mModelMatrix.setColumn(3, QVector4D(x, y, z, 1));
    mPosition = QVector3D(x, y, z);
    mColor = QVector3D(1, 1, 1);
}

const QMatrix4x4 &Object::modelMatrix() const
{
    return mModelMatrix;
}

void Object::setModelMatrix(const QMatrix4x4 &newModelMatrix)
{
    QVector4D translation = newModelMatrix.column(3);
    mPosition = QVector3D(translation.x(), translation.y(), translation.z());
    mModelMatrix = newModelMatrix;
}

const QVector3D &Object::position() const
{
    return mPosition;
}

void Object::setPosition(float x, float y, float z)
{
    mModelMatrix.setColumn(3, QVector4D(x, y, z, 1));
    mPosition = QVector3D(x, y, z);
}

const QVector3D &Object::color() const
{
    return mColor;
}

void Object::setColor(float r, float g, float b)
{
    mColor = QVector3D(r, g, b);
}

void Object::rotate(const QQuaternion &deltaRotation)
{
    mModelMatrix.rotate(deltaRotation);
}

void Object::scale(float x, float y, float z)
{
    mModelMatrix.scale(x, y, z);
}

void Object::scale(float factor)
{
    mModelMatrix.scale(factor);
}

#include "Object.h"

Object::Object()
    : mColor(QVector3D(0.5, 0.5, 0.5))
{
    mModelMatrix.setToIdentity();
}

const QMatrix4x4 &Object::modelMatrix() const
{
    return mModelMatrix;
}

void Object::setModelMatrix(const QMatrix4x4 &newModelMatrix)
{
    mModelMatrix = newModelMatrix;
}

void Object::setPosition(const QVector3D &newPostion)
{
    mModelMatrix.setColumn(3, QVector4D(newPostion.x(), newPostion.y(), newPostion.z(), 1));
}

const QVector3D &Object::color() const
{
    return mColor;
}

void Object::setColor(const QVector3D &newColor)
{
    mColor = newColor;
}

void Object::setColor(float x, float y, float z)
{
    mColor = QVector3D(x, y, z);
}

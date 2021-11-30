#include "Object.h"

Object::Object()
    : mPosition(0, 0, 0)
    , mColor(1, 1, 1)
{}

Object::~Object() { mTransformationMatrix.setToIdentity(); }

const QMatrix4x4 &Object::transformationMatrix() const { return mTransformationMatrix; }

void Object::setTransformationMatrix(const QMatrix4x4 &newTransformationMatrix)
{
    mTransformationMatrix = newTransformationMatrix;
    QVector4D translation = newTransformationMatrix.column(3);
    mPosition = QVector3D(translation.x(), translation.y(), translation.z());
}

const QVector3D &Object::position() const { return mPosition; }

void Object::setPosition(float x, float y, float z)
{
    mTransformationMatrix.setColumn(3, QVector4D(x, y, z, 1));
    mPosition = QVector3D(x, y, z);
}

void Object::rotate(const QQuaternion &deltaRotation) { mTransformationMatrix.rotate(deltaRotation); }

void Object::scale(float x, float y, float z) { mTransformationMatrix.scale(x, y, z); }

void Object::scale(float factor) { mTransformationMatrix.scale(factor); }

const QVector3D &Object::color() const { return mColor; }

void Object::setColor(float r, float g, float b) { mColor = QVector3D(r, g, b); }

void Object::setColor(const QVector3D &newColor) { mColor = newColor; }

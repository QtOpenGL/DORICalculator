#ifndef OBJECT_H
#define OBJECT_H

#include <QMatrix4x4>

class Object
{
public:
    Object(float x = 0, float y = 0, float z = 0);

    virtual const QMatrix4x4 &modelMatrix() const;
    virtual void setModelMatrix(const QMatrix4x4 &newModelMatrix);

    virtual const QVector3D &position() const;
    virtual void setPosition(float x, float y, float z);

    virtual const QVector3D &color() const;
    virtual void setColor(float r, float g, float b);

    virtual void rotate(const QQuaternion &deltaRotation);

    virtual void scale(float x, float y, float z);
    virtual void scale(float factor);

protected:
    QMatrix4x4 mModelMatrix;
    QVector3D mPosition;
    QVector3D mColor;
};

#endif // OBJECT_H

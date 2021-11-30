#ifndef OBJECT_H
#define OBJECT_H

#include <QMatrix4x4>

class Object
{
public:
    Object();
    virtual ~Object();

    const QMatrix4x4 &transformationMatrix() const;
    virtual void setTransformationMatrix(const QMatrix4x4 &newTransformationMatrix);

    const QVector3D &position() const;
    virtual void setPosition(float x, float y, float z);

    void rotate(const QQuaternion &deltaRotation);

    void scale(float x, float y, float z);
    void scale(float factor);

    const QVector3D &color() const;
    void setColor(float r, float g, float b);

protected:
    QMatrix4x4 mTransformationMatrix;
    QVector3D mPosition;
    QVector3D mColor;
};

#endif // OBJECT_H

#ifndef OBJECT_H
#define OBJECT_H

#include <QMatrix4x4>
#include <QQuaternion>
#include <QString>

class Object
{
public:
    enum Type { Capsule, Cone, Cube, Cylinder, Dome, Plane, Pyramid, Sphere, Suzanne, Tetrahedron, Torus, TorusKnot };

    Object(Type type);

    Type type() const;

    const QMatrix4x4 &transformationMatrix() const;
    virtual void setTransformationMatrix(const QMatrix4x4 &newTransformationMatrix);

    const QVector3D &position() const;
    virtual void setPosition(float x, float y, float z);

    void rotate(const QQuaternion &deltaRotation);

    void scale(float x, float y, float z);
    void scale(float factor);

    const QVector3D &color() const;
    void setColor(float r, float g, float b);
    void setColor(const QVector3D &newColor);

protected:
    Type mType;
    QMatrix4x4 mTransformationMatrix;
    QVector3D mPosition;
    QVector3D mColor;
};

#endif // OBJECT_H

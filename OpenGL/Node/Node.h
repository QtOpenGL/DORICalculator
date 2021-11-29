#ifndef NODE_H
#define NODE_H

#include <QMatrix4x4>

class Node
{
public:
    Node();

    virtual const QMatrix4x4 &transformationMatrix() const;
    virtual void setTransformationMatrix(const QMatrix4x4 &newTransformationMatrix);

    virtual const QVector3D &position() const;
    virtual void setPosition(float x, float y, float z);

    virtual void rotate(const QQuaternion &deltaRotation);

    virtual void scale(float x, float y, float z);
    virtual void scale(float factor);

protected:
    QMatrix4x4 mTransformationMatrix;
    QVector3D mPosition;
};

#endif // NODE_H

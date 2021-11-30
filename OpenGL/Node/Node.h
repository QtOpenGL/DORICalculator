#ifndef NODE_H
#define NODE_H

#include <QMatrix4x4>

class Node
{
public:
    Node();

    const QMatrix4x4 &transformationMatrix() const { return mTransformationMatrix; }
    virtual void setTransformationMatrix(const QMatrix4x4 &newTransformationMatrix);

    const QVector3D &position() const { return mPosition; }
    virtual void setPosition(float x, float y, float z);

    void rotate(const QQuaternion &deltaRotation) { mTransformationMatrix.rotate(deltaRotation); }

    void scale(float x, float y, float z) { mTransformationMatrix.scale(x, y, z); }
    void scale(float factor) { mTransformationMatrix.scale(factor); }

    const QVector3D &color() const { return mColor; }
    void setColor(float r, float g, float b) { mColor = QVector3D(r, g, b); }

protected:
    QMatrix4x4 mTransformationMatrix;
    QVector3D mPosition;
    QVector3D mColor;
};

#endif // NODE_H

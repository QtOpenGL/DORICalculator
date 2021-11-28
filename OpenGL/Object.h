#ifndef OBJECT_H
#define OBJECT_H

#include <QMatrix4x4>

class Object
{
public:
    Object();

    virtual const QMatrix4x4 &modelMatrix() const;
    virtual void setModelMatrix(const QMatrix4x4 &newModelMatrix);
    virtual void setPosition(const QVector3D &newPostion);

    virtual const QVector3D &color() const;
    virtual void setColor(const QVector3D &newColor);
    virtual void setColor(float x, float y, float z);

protected:
    QMatrix4x4 mModelMatrix;
    QVector3D mColor;
};

#endif // OBJECT_H

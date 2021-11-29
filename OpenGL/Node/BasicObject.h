#ifndef BASICOBJECT_H
#define BASICOBJECT_H

#include <OpenGL/Node/Node.h>

class BasicObject : public Node
{
public:
    enum Type { Cuboid, Plane };

    BasicObject(Type type);

    virtual const QVector3D &color() const;
    virtual void setColor(float r, float g, float b);

    virtual void scale(float x, float y, float z);
    virtual void scale(float factor);

    Type type() const;
    void setType(Type newType);

protected:
    Type mType;
    QVector3D mColor;
};

#endif // BASICOBJECT_H
#ifndef BASICOBJECT_H
#define BASICOBJECT_H

#include <OpenGL/Node/Node.h>

class BasicObject : public Node
{
public:
    enum Type { Cuboid, Plane };

    BasicObject(Type type);

    Type type() const { return mType; };
    void setType(Type newType) { mType = newType; };

protected:
    Type mType;
};

#endif // BASICOBJECT_H

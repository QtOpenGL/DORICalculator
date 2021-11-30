#ifndef MODEL_H
#define MODEL_H

#include "Object.h"

#include <QMatrix4x4>
#include <QQuaternion>
#include <QString>

class Model : public Object
{
public:
    enum Type { Capsule, Cone, Cube, Cylinder, Dome, Plane, Pyramid, Sphere, Suzanne, Tetrahedron, Torus, TorusKnot };

    Model(Type type);

    Type type() const;

protected:
    Type mType;
};

#endif // MODEL_H

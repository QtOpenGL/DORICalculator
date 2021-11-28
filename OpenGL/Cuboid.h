#ifndef CUBOID_H
#define CUBOID_H
#include "Object.h"

#include <QMatrix4x4>

class Cuboid : public Object
{
public:
    Cuboid(QVector3D position = QVector3D(0, 0, 0));
    Cuboid(float x, float y, float z);
};

#endif // CUBOID_H

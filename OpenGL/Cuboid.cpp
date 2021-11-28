#include "Cuboid.h"

Cuboid::Cuboid(QVector3D position)
{
    setPosition(position);
}

Cuboid::Cuboid(float x, float y, float z)
    : Cuboid(QVector3D(x, y, z))
{}
